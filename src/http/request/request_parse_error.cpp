/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parse_error.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 00:00:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/15 00:00:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request_parse_error.hpp"

namespace
{
struct ErrorDefinition
{
    RequestParseError::Id id;
    int status;
    const char* message;
};

// =====================================================================
// 	REQUEST PARSE ERROR TABLE
// =====================================================================
const ErrorDefinition errors[] = {
    { RequestParseError::INVALID_LINE_ENDING, 400, "Invalid line ending" },
    { RequestParseError::UNEXPECTED_END_OF_HEADERS, 400, "Unexpected end of request headers" },
    { RequestParseError::INVALID_METHOD_SYNTAX, 400, "Invalid HTTP method syntax" },
    { RequestParseError::INVALID_REQUEST_TARGET, 400, "Invalid request target" },
    { RequestParseError::INVALID_VERSION_SYNTAX, 400, "Invalid HTTP version syntax" },
    { RequestParseError::INVALID_HEADER_KEY, 400, "Invalid header key" },
    { RequestParseError::INVALID_HEADER_VALUE, 400, "Invalid header value" },
    { RequestParseError::INVALID_REQUEST_LINE, 400, "Invalid request line" },
    { RequestParseError::UNSUPPORTED_VERSION, 505, "Unsupported HTTP version" },
    { RequestParseError::UNSUPPORTED_METHOD, 501, "Unsupported HTTP method" },
    { RequestParseError::OBSOLETE_HEADER_FOLDING, 400, "Obsolete header folding is not supported" },
    { RequestParseError::MISSING_HEADER_COLON, 400, "Missing header colon" },
    { RequestParseError::INVALID_TRANSFER_TOKEN, 400, "Invalid Transfer-Encoding token" },
    { RequestParseError::INVALID_TRANSFER_PARAMETER, 400, "Invalid Transfer-Encoding parameter" },
    { RequestParseError::UNTERMINATED_TRANSFER_PARAMETER, 400, "Unterminated Transfer-Encoding parameter" },
    { RequestParseError::CHUNKED_PARAMETERS, 400, "Chunked coding does not accept parameters" },
    { RequestParseError::INVALID_TRANSFER_LIST, 400, "Invalid Transfer-Encoding list" },
    { RequestParseError::FINAL_CODING_NOT_CHUNKED, 400, "Final request transfer coding must be chunked" },
    { RequestParseError::REPEATED_CHUNKED_CODING, 400, "Chunked coding must appear exactly once and last" },
    { RequestParseError::UNSUPPORTED_TRANSFER_CODING, 501, "Unsupported transfer coding" },
    { RequestParseError::INVALID_CONNECT_AUTHORITY, 400, "Invalid CONNECT authority" },
    { RequestParseError::INVALID_URI, 400, "Invalid URI" },
    { RequestParseError::INVALID_URI_QUERY, 400, "Invalid URI query" },
    { RequestParseError::INVALID_TARGET_FORM, 400, "Invalid request-target form" },
    { RequestParseError::INVALID_ABSOLUTE_AUTHORITY, 400, "Invalid absolute URI authority" },
    { RequestParseError::MISSING_HOST, 400, "Missing Host header" },
    { RequestParseError::INVALID_HOST, 400, "Invalid or duplicate Host header" },
    { RequestParseError::AMBIGUOUS_FRAMING, 400, "Ambiguous Transfer-Encoding and Content-Length" },
    { RequestParseError::INVALID_CONTENT_LENGTH, 400, "Invalid or conflicting Content-Length" },
    { RequestParseError::UNEXPECTED_END_OF_BODY, 400, "Unexpected end of request body" },
    { RequestParseError::INVALID_CHUNK_SIZE, 400, "Invalid chunk size" },
    { RequestParseError::INVALID_CHUNK_EXTENSION, 400, "Invalid chunk extension" },
    { RequestParseError::INVALID_CHUNK_DELIMITER, 400, "Invalid chunk data delimiter" },
    { RequestParseError::FORBIDDEN_TRAILER, 400, "Forbidden request trailer field" },
    { RequestParseError::REQUEST_LINE_TOO_LONG, 414, "Request line exceeds configured limit" },
    { RequestParseError::HEADERS_TOO_LARGE, 431, "Request fields exceed configured limit" },
    { RequestParseError::BODY_TOO_LARGE, 413, "Request body exceeds configured limit" },
    { RequestParseError::BUFFER_LIMIT_EXCEEDED, 413, "Pending input exceeds configured limit" },
    { RequestParseError::CHUNK_METADATA_TOO_LARGE, 413, "Chunk metadata exceeds configured limit" },
};

typedef char ErrorTableCoversEveryId[
    sizeof(errors) / sizeof(errors[0]) == RequestParseError::COUNT ? 1 : -1];

const ErrorDefinition& definition(RequestParseError::Id id)
{
    for (size_t i = 0; i < sizeof(errors) / sizeof(errors[0]); ++i)
        if (errors[i].id == id)
            return errors[i];
    static const ErrorDefinition unknown = { RequestParseError::COUNT, 500, "Unknown request parse error" };
    return unknown;
}
}

RequestParseError::RequestParseError(Id id)
    : std::runtime_error(definition(id).message), _errorCode(definition(id).status)
{
}
