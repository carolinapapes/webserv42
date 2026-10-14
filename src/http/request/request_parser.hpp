/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parser.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 00:00:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/15 00:00:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "schemas.hpp"

// Byte limits include CRLF where applicable. Zero body bytes forbids a body.
struct RequestParserLimits
{
    size_t maxRequestLineBytes;
    size_t maxHeaderBytes;
    size_t maxBodyBytes;
    size_t maxPendingBytes;
    size_t maxChunkLineBytes;
    size_t maxChunkMetadataBytes;

    RequestParserLimits();
};

class RequestParser
{
    public:
        explicit RequestParser(
            const RequestParserLimits& limits = RequestParserLimits());

        RequestParseResult::Status feed(const std::string& bytes,
                                        bool endOfInput = false);
        const RequestParseResult& result() const;
        const std::string& pendingData() const;
        RequestParseResult::Status next();
        void reset();

    private:
        enum State
        {
            REQUEST_LINE, HEADER_LINES, FIXED_BODY,
            CHUNK_SIZE, CHUNK_DATA, CHUNK_CRLF, TRAILER_LINES
        };

        RequestParserLimits _limits;
        RequestParseResult _result;
        State _state;
        Headers _headers;
        std::string _body;
        std::string _buffer;
        size_t _offset;
        size_t _scan;
        size_t _remaining;
        size_t _headerBytes;
        size_t _chunkMetadataBytes;
        bool _endOfInput;

        void startRequest();
        void process();
        bool step();
        bool readLine(std::string& line, size_t limit,
                       RequestParseError::Id limitError);
        void selectBody();
        bool readBody();
        void readChunkSize(const std::string& line);
        bool readChunkDelimiter();
        void readTrailer(const std::string& line);
        void complete();
        void fail(const RequestParseError& error);
        void compact();
};
