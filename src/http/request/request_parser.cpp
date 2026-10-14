/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parser.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 00:00:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/15 00:00:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request_parser.hpp"
#include "request_syntax.hpp"
#include "field_validators.hpp"
#include <algorithm>
#include <limits>

namespace
{
// =====================================================================
// 	CHUNK EXTENSION SYNTAX
// =====================================================================
void skipWhitespace(const std::string& text, size_t& pos)
{
    while (pos < text.size() && (text[pos] == ' ' || text[pos] == '\t'))
        ++pos;
}

bool tokenByte(unsigned char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
        || (c >= 'a' && c <= 'z')
        || std::string("!#$%&'*+-.^_`|~").find(c) != std::string::npos;
}

void token(const std::string& text, size_t& pos)
{
    const size_t start = pos;
    while (pos < text.size() && tokenByte(text[pos]))
        ++pos;
    if (start == pos)
        throw RequestParseError(RequestParseError::INVALID_CHUNK_EXTENSION);
}

void extensionValue(const std::string& text, size_t& pos)
{
    if (pos == text.size() || text[pos] != '"')
    {
        token(text, pos);
        return;
    }
    ++pos;
    while (pos < text.size())
    {
        unsigned char c = text[pos++];
        if (c == '"')
            return;
        if (c == '\\')
        {
            if (pos == text.size())
                break;
            c = text[pos++];
        }
        if ((c < 32 && c != '\t') || c == 127)
            throw RequestParseError(RequestParseError::INVALID_CHUNK_EXTENSION);
    }
    throw RequestParseError(RequestParseError::INVALID_CHUNK_EXTENSION);
}

void extensions(const std::string& text, size_t pos)
{
    while (pos < text.size())
    {
        skipWhitespace(text, pos);
        if (pos == text.size() || text[pos++] != ';')
            throw RequestParseError(RequestParseError::INVALID_CHUNK_EXTENSION);
        skipWhitespace(text, pos);
        token(text, pos);
        const size_t endName = pos;
        skipWhitespace(text, pos);
        if (pos < text.size() && text[pos] == '=')
        {
            ++pos;
            skipWhitespace(text, pos);
            extensionValue(text, pos);
        }
        else
            pos = endName;
    }
}

int hexDigit(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}
}

// =====================================================================
// 	PARSER LIFETIME AND OWNED INPUT
// =====================================================================
RequestParserLimits::RequestParserLimits()
    : maxRequestLineBytes(8192), maxHeaderBytes(65536),
      maxBodyBytes(1024 * 1024), maxPendingBytes(2 * 1024 * 1024),
      maxChunkLineBytes(8192), maxChunkMetadataBytes(65536)
{
}

RequestParser::RequestParser(const RequestParserLimits& limits)
    : _limits(limits), _endOfInput(false)
{
    startRequest();
}

void RequestParser::startRequest()
{
    _result = RequestParseResult();
    _state = REQUEST_LINE;
    _headers = Headers();
    _body.clear();
    _offset = 0;
    _scan = 0;
    _remaining = 0;
    _headerBytes = 0;
    _chunkMetadataBytes = 0;
}

void RequestParser::reset()
{
    _buffer.clear();
    _endOfInput = false;
    startRequest();
}

const RequestParseResult& RequestParser::result() const { return _result; }
const std::string& RequestParser::pendingData() const { return _buffer; }

RequestParseResult::Status RequestParser::feed(const std::string& bytes,
                                             bool endOfInput)
{
    if (_result.status == RequestParseResult::ERROR)
        return _result.status;
    if (bytes.size() > _limits.maxPendingBytes - _buffer.size())
    {
        fail(RequestParseError(RequestParseError::BUFFER_LIMIT_EXCEEDED));
        return _result.status;
    }
    _buffer.append(bytes);
    _endOfInput = _endOfInput || endOfInput;
    if (_result.status == RequestParseResult::INCOMPLETE)
        process();
    return _result.status;
}

RequestParseResult::Status RequestParser::next()
{
    if (_result.status != RequestParseResult::COMPLETE)
        return _result.status;
    startRequest();
    if (!_buffer.empty())
        process();
    return _result.status;
}

void RequestParser::fail(const RequestParseError& error)
{
    _result.status = RequestParseResult::ERROR;
    _result.request.setErrorCode(error.getErrorCode());
    _result.message = error.what();
    _buffer.clear();
    _body.clear();
    _offset = 0;
    _scan = 0;
}

void RequestParser::complete()
{
    _result.request.setBody(_body);
    _body.clear();
    _result.status = RequestParseResult::COMPLETE;
}

void RequestParser::compact()
{
    _buffer.erase(0, _offset);
    _scan -= _offset;
    _offset = 0;
}

// =====================================================================
// 	BOUNDED LINE READER AND STATE MACHINE
// =====================================================================
bool RequestParser::readLine(std::string& line, size_t limit,
                             RequestParseError::Id limitError)
{
    while (_scan < _buffer.size())
    {
        const char c = _buffer[_scan];
        if (c == '\n')
            throw RequestParseError(RequestParseError::INVALID_LINE_ENDING);
        if (c == '\r')
        {
            if (_scan + 1 == _buffer.size())
                break;
            if (_buffer[_scan + 1] != '\n')
                throw RequestParseError(RequestParseError::INVALID_LINE_ENDING);
            if (_scan - _offset + 2 > limit)
                throw RequestParseError(limitError);
            line.assign(_buffer, _offset, _scan - _offset);
            _offset = _scan + 2;
            _scan = _offset;
            return true;
        }
        if (_scan - _offset >= limit)
            throw RequestParseError(limitError);
        ++_scan;
    }
    const size_t bytes = _buffer.size() - _offset;
    const size_t needed = bytes && _buffer[_buffer.size() - 1] == '\r' ? 1 : 2;
    if (bytes > limit || needed > limit - bytes)
        throw RequestParseError(limitError);
    return false;
}

void RequestParser::process()
{
    try
    {
        while (_result.status == RequestParseResult::INCOMPLETE && step())
        {
        }
        if (_endOfInput && _result.status == RequestParseResult::INCOMPLETE)
            throw RequestParseError(_state == REQUEST_LINE || _state == HEADER_LINES
                ? RequestParseError::UNEXPECTED_END_OF_HEADERS
                : RequestParseError::UNEXPECTED_END_OF_BODY);
        compact();
    }
    catch (const RequestParseError& error)
    {
        fail(error);
    }
}

bool RequestParser::step()
{
    std::string line;
    switch (_state)
    {
        case REQUEST_LINE:
            if (!readLine(line, _limits.maxRequestLineBytes,
                          RequestParseError::REQUEST_LINE_TOO_LONG))
                return false;
            _result.request.setControlData(RequestSyntax::validateControlData(line));
            _state = HEADER_LINES;
            return true;
        case HEADER_LINES:
        case TRAILER_LINES:
            if (!readLine(line, _limits.maxHeaderBytes - _headerBytes,
                          RequestParseError::HEADERS_TOO_LARGE))
                return false;
            _headerBytes += line.size() + 2;
            if (_state == TRAILER_LINES)
                readTrailer(line);
            else if (line.empty())
                selectBody();
            else
                RequestSyntax::validateHeader(line, _headers);
            return true;
        case FIXED_BODY:
        case CHUNK_DATA:
            return readBody();
        case CHUNK_SIZE:
            if (!readLine(line, std::min(_limits.maxChunkLineBytes,
                            _limits.maxChunkMetadataBytes - _chunkMetadataBytes),
                          RequestParseError::CHUNK_METADATA_TOO_LARGE))
                return false;
            _chunkMetadataBytes += line.size() + 2;
            readChunkSize(line);
            return true;
        case CHUNK_CRLF:
            return readChunkDelimiter();
    }
    return false;
}

// =====================================================================
// 	MESSAGE BODY FRAMING
// =====================================================================
void RequestParser::selectBody()
{
    specificHeadersValidation(_headers);
    _result.request.setHeaders(_headers);
    if (_headers.has("Transfer-Encoding"))
        _state = CHUNK_SIZE;
    else if (_headers.has("Content-Length"))
    {
        if (!parseContentLength(_headers.get("Content-Length"), _remaining))
            throw RequestParseError(RequestParseError::INVALID_CONTENT_LENGTH);
        if (_remaining > _limits.maxBodyBytes)
            throw RequestParseError(RequestParseError::BODY_TOO_LARGE);
        _state = FIXED_BODY;
        if (_remaining == 0)
            complete();
    }
    else
        complete();
}

bool RequestParser::readBody()
{
    const size_t count = std::min(_remaining, _buffer.size() - _offset);
    if (count == 0)
        return false;
    _body.append(_buffer, _offset, count);
    _offset += count;
    _scan = _offset;
    _remaining -= count;
    if (_remaining == 0)
    {
        if (_state == FIXED_BODY)
            complete();
        else
            _state = CHUNK_CRLF;
    }
    return true;
}

void RequestParser::readChunkSize(const std::string& line)
{
    size_t size = 0;
    size_t pos = 0;
    int digit;
    while (pos < line.size() && (digit = hexDigit(line[pos])) >= 0)
    {
        if (size > (std::numeric_limits<size_t>::max() - digit) / 16)
            throw RequestParseError(RequestParseError::BODY_TOO_LARGE);
        size = size * 16 + digit;
        ++pos;
    }
    if (pos == 0)
        throw RequestParseError(RequestParseError::INVALID_CHUNK_SIZE);
    extensions(line, pos);
    if (size > _limits.maxBodyBytes - _body.size())
        throw RequestParseError(RequestParseError::BODY_TOO_LARGE);
    _remaining = size;
    _state = size == 0 ? TRAILER_LINES : CHUNK_DATA;
}

bool RequestParser::readChunkDelimiter()
{
    const size_t available = _buffer.size() - _offset;
    if (available && _buffer[_offset] != '\r')
        throw RequestParseError(RequestParseError::INVALID_CHUNK_DELIMITER);
    if (available < 2)
        return false;
    if (_buffer[_offset + 1] != '\n')
        throw RequestParseError(RequestParseError::INVALID_CHUNK_DELIMITER);
    if (_limits.maxChunkMetadataBytes - _chunkMetadataBytes < 2)
        throw RequestParseError(RequestParseError::CHUNK_METADATA_TOO_LARGE);
    _chunkMetadataBytes += 2;
    _offset += 2;
    _scan = _offset;
    _state = CHUNK_SIZE;
    return true;
}

void RequestParser::readTrailer(const std::string& line)
{
    if (line.empty())
    {
        complete();
        return;
    }
    Headers trailer;
    RequestSyntax::validateHeader(line, trailer);
    const char* forbidden[] = {
        "Host", "Content-Length", "Transfer-Encoding", "Connection", "Trailer",
        "TE", "Authorization", "Proxy-Authorization", "Content-Encoding",
        "Content-Type", "Content-Range"
    };
    for (size_t i = 0; i < sizeof(forbidden) / sizeof(forbidden[0]); ++i)
        if (trailer.has(forbidden[i]))
            throw RequestParseError(RequestParseError::FORBIDDEN_TRAILER);
    // Supported policy: validate and discard trailers; never merge headers.
}
