/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_validators.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:11:28 by capapes           #+#    #+#             */
/*   Updated: 2025/11/28 20:41:18 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "field_validators.hpp"
#include "uri.hpp"
#include <arpa/inet.h>
#include <climits>

namespace
{
bool asciiAlnum(unsigned char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

bool tokenChar(unsigned char c)
{
    return asciiAlnum(c) || std::string("!#$%&'*+-.^_`|~").find(c) != std::string::npos;
}

bool hexDigit(unsigned char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

std::string lowerAscii(std::string value)
{
    for (size_t i = 0; i < value.size(); ++i)
        if (value[i] >= 'A' && value[i] <= 'Z')
            value[i] += 'a' - 'A';
    return value;
}

bool uriText(const std::string& value, const std::string& punctuation)
{
    for (size_t i = 0; i < value.size(); ++i)
    {
        const unsigned char c = value[i];
        if (c == '%')
        {
            if (value.size() - i < 3 || !hexDigit(value[i + 1]) || !hexDigit(value[i + 2]))
                return false;
            i += 2;
        }
        else if (!asciiAlnum(c) && punctuation.find(c) == std::string::npos)
            return false;
    }
    return true;
}

bool decimal(const std::string& value, size_t limit, size_t& result)
{
    result = 0;
    if (value.empty())
        return false;
    size_t number = 0;
    for (size_t i = 0; i < value.size(); ++i)
    {
        if (value[i] < '0' || value[i] > '9')
            return false;
        const size_t digit = value[i] - '0';
        if (number > limit / 10 || (number == limit / 10 && digit > limit % 10))
            return false;
        number = number * 10 + digit;
    }
    result = number;
    return true;
}

bool ipLiteral(const std::string& address)
{
    if (address.empty() || address.find('\0') != std::string::npos)
        return false;
    if (address[0] == 'v' || address[0] == 'V')
    {
        const size_t dot = address.find('.');
        if (dot == std::string::npos || dot < 2 || dot + 1 == address.size())
            return false;
        for (size_t i = 1; i < dot; ++i)
            if (!hexDigit(address[i]))
                return false;
        for (size_t i = dot + 1; i < address.size(); ++i)
            if (!asciiAlnum(address[i])
                && std::string("-._~!$&'()*+,;=:").find(address[i]) == std::string::npos)
                return false;
        return true;
    }
    struct in6_addr bytes;
    return inet_pton(AF_INET6, address.c_str(), &bytes) == 1;
}

bool authority(const std::string& value, bool requirePort = false)
{
    if (value.empty())
        return false;
    size_t portStart = std::string::npos;
    if (value[0] == '[')
    {
        const size_t close = value.find(']');
        if (close == std::string::npos || !ipLiteral(value.substr(1, close - 1)))
            return false;
        if (close + 1 != value.size())
        {
            if (value[close + 1] != ':')
                return false;
            portStart = close + 2;
        }
    }
    else
    {
        const size_t colon = value.find(':');
        const std::string name = value.substr(0, colon);
        if (name.empty() || !uriText(name, "-._~!$&'()*+,;="))
            return false;
        if (colon != std::string::npos)
            portStart = colon + 1;
    }
    if (portStart == std::string::npos)
        return !requirePort;
    size_t port = 0;
    return decimal(value.substr(portStart), 65535, port);
}

void skipWhitespace(const std::string& text, size_t& pos)
{
    while (pos < text.size() && (text[pos] == ' ' || text[pos] == '\t'))
        ++pos;
}

std::string readToken(const std::string& text, size_t& pos)
{
    const size_t start = pos;
    while (pos < text.size() && tokenChar(text[pos]))
        ++pos;
    if (start == pos)
        throw RequestParseError(RequestParseError::INVALID_TRANSFER_TOKEN);
    return text.substr(start, pos - start);
}

void readParameterValue(const std::string& text, size_t& pos)
{
    if (pos == text.size() || text[pos] != '"')
    {
        readToken(text, pos);
        return;
    }
    ++pos;
    while (pos < text.size())
    {
        if (text[pos] == '"')
        {
            ++pos;
            return;
        }
        if (text[pos] == '\\')
        {
            ++pos;
            if (pos == text.size())
                break;
        }
        const unsigned char c = text[pos++];
        if ((c < 32 && c != '\t') || c == 127)
            throw RequestParseError(RequestParseError::INVALID_TRANSFER_PARAMETER);
    }
    throw RequestParseError(RequestParseError::UNTERMINATED_TRANSFER_PARAMETER);
}

void validateTransferEncoding(const std::vector<std::string>& fields)
{
    std::string text;
    for (size_t i = 0; i < fields.size(); ++i)
    {
        if (i != 0)
            text += ",";
        text += fields[i];
    }
    std::vector<std::string> codings;
    size_t pos = 0;
    while (pos < text.size())
    {
        skipWhitespace(text, pos);
        if (pos == text.size())
            break;
        if (text[pos] == ',')
        {
            ++pos;
            continue;
        }
        const std::string coding = lowerAscii(readToken(text, pos));
        skipWhitespace(text, pos);
        while (pos < text.size() && text[pos] == ';')
        {
            if (coding == "chunked")
                throw RequestParseError(RequestParseError::CHUNKED_PARAMETERS);
            ++pos;
            skipWhitespace(text, pos);
            readToken(text, pos);
            skipWhitespace(text, pos);
            if (pos == text.size() || text[pos++] != '=')
                throw RequestParseError(RequestParseError::INVALID_TRANSFER_PARAMETER);
            skipWhitespace(text, pos);
            readParameterValue(text, pos);
            skipWhitespace(text, pos);
        }
        codings.push_back(coding);
        if (pos == text.size())
            break;
        if (text[pos++] != ',')
            throw RequestParseError(RequestParseError::INVALID_TRANSFER_LIST);
    }
    if (codings.empty() || codings.back() != "chunked")
        throw RequestParseError(RequestParseError::FINAL_CODING_NOT_CHUNKED);
    for (size_t i = 0; i + 1 < codings.size(); ++i)
        if (codings[i] == "chunked")
            throw RequestParseError(RequestParseError::REPEATED_CHUNKED_CODING);
    if (codings.size() != 1)
        throw RequestParseError(RequestParseError::UNSUPPORTED_TRANSFER_CODING);
}
}

// =====================================================================
// 					VALIDATORS FOR REQUEST HEADERS FIELDS
// =====================================================================
bool isValidKey(const std::string& key)
{
    if (key.empty() || key.size() > 256)
        return false;
    for (size_t i = 0; i < key.size(); ++i)
        if (!tokenChar(key[i]))
            return false;
    return true;
}

bool isValidValue(const std::string& value)
{
    if (value.size() > 8192)
        return false;
    for (size_t i = 0; i < value.size(); ++i)
    {
        const unsigned char c = value[i];
        if ((c < 32 && c != '\t') || c == 127)
            return false;
    }
    return true;
}

// =====================================================================
// 					VALIDATORS FOR CONTROL DATA FIELDS
// =====================================================================
bool validMethod(const std::string& method)
{
    if (method.empty())
        return false;
    for (size_t i = 0; i < method.size(); ++i)
        if (!tokenChar(method[i]))
            return false;
    return true;
}

bool isSupportedMethod(const std::string& method)
{
    return method == "GET" || method == "POST" || method == "PUT" || method == "DELETE";
}

bool isValidRequest(const std::string& target) { return !target.empty(); }

bool isValidProtocolSyntax(const std::string& version)
{
    return version.size() == 8 && version.compare(0, 5, "HTTP/") == 0
        && version[5] >= '0' && version[5] <= '9' && version[6] == '.'
        && version[7] >= '0' && version[7] <= '9';
}

bool isValidProtocol(const std::string& version) { return version == "HTTP/1.1"; }

// =====================================================================
// 					VALIDATORS FOR HEADER FIELDS
// =====================================================================
bool parseContentLength(const std::string& value, size_t& length)
{
    return decimal(value, INT_MAX, length);
}

bool isValidContentLength(const std::string& value)
{
    size_t length = 0;
    return parseContentLength(value, length);
}

// =====================================================================
// 	ODDLY SPECIFIC VALIDATORS VALIDATORS
// =====================================================================
void specificControlDataValidation(const std::string& target, const std::string& method)
{
    if (method == "CONNECT")
    {
        if (!authority(target, true))
            throw RequestParseError(RequestParseError::INVALID_CONNECT_AUTHORITY);
        return;
    }
    if (target == "*" && method == "OPTIONS")
        return;
    if (target.empty() || !uriText(target, "-._~:/?[]@!$&'()*+,;="))
        throw RequestParseError(RequestParseError::INVALID_URI);
    const size_t query = target.find('?');
    if (query != std::string::npos && !uriText(target.substr(query + 1), "-._~!$&'()*+,;=:@/?"))
        throw RequestParseError(RequestParseError::INVALID_URI_QUERY);

    std::string path;
    if (target[0] == '/')
        path = target;
    else
    {
        const std::string lower = lowerAscii(target);
        const size_t start = lower.compare(0, 7, "http://") == 0 ? 7
                           : lower.compare(0, 8, "https://") == 0 ? 8 : 0;
        if (start == 0)
            throw RequestParseError(RequestParseError::INVALID_TARGET_FORM);
        const size_t end = target.find_first_of("/?", start);
        const std::string host = target.substr(start, end == std::string::npos ? end : end - start);
        if (!authority(host))
            throw RequestParseError(RequestParseError::INVALID_ABSOLUTE_AUTHORITY);
        path = end == std::string::npos ? "/" : target.substr(end);
        if (path[0] == '?')
            path = "/" + path;
    }
    try
    {
        // The existing URI parser validates path/query syntax. A fixed authority
        // avoids feeding unchecked authority tokens to its legacy constructors.
        URI parsed("http://request.invalid" + path);
    }
    catch (const std::exception& error)
    {
        throw RequestParseError(RequestParseError::INVALID_URI);
    }
}

void specificHeadersValidation(const Headers& headers)
{
    const std::vector<std::string> hosts = headers.getAll("Host");
    if (hosts.empty())
        throw RequestParseError(RequestParseError::MISSING_HOST);
    if (hosts.size() != 1 || !authority(hosts[0]))
        throw RequestParseError(RequestParseError::INVALID_HOST);
    if (headers.has("Content-Length") && headers.has("Transfer-Encoding"))
        throw RequestParseError(RequestParseError::AMBIGUOUS_FRAMING);

    const std::vector<std::string> lengths = headers.getAll("Content-Length");
    size_t firstLength = 0;
    for (size_t i = 0; i < lengths.size(); ++i)
    {
        size_t length = 0;
        if (!parseContentLength(lengths[i], length) || (i != 0 && length != firstLength))
            throw RequestParseError(RequestParseError::INVALID_CONTENT_LENGTH);
        firstLength = length;
    }
    if (headers.has("Transfer-Encoding"))
        validateTransferEncoding(headers.getAll("Transfer-Encoding"));
}
