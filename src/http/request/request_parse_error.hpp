/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parse_error.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 23:50:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/14 23:50:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdexcept>
#include <string>

class RequestParseError : public std::runtime_error
{
    private:
        int _errorCode;

    public:
        enum Id
        {
            INVALID_LINE_ENDING,
            UNEXPECTED_END_OF_HEADERS,
            INVALID_METHOD_SYNTAX,
            INVALID_REQUEST_TARGET,
            INVALID_VERSION_SYNTAX,
            INVALID_HEADER_KEY,
            INVALID_HEADER_VALUE,
            INVALID_REQUEST_LINE,
            UNSUPPORTED_VERSION,
            UNSUPPORTED_METHOD,
            OBSOLETE_HEADER_FOLDING,
            MISSING_HEADER_COLON,
            INVALID_TRANSFER_TOKEN,
            INVALID_TRANSFER_PARAMETER,
            UNTERMINATED_TRANSFER_PARAMETER,
            CHUNKED_PARAMETERS,
            INVALID_TRANSFER_LIST,
            FINAL_CODING_NOT_CHUNKED,
            REPEATED_CHUNKED_CODING,
            UNSUPPORTED_TRANSFER_CODING,
            INVALID_CONNECT_AUTHORITY,
            INVALID_URI,
            INVALID_URI_QUERY,
            INVALID_TARGET_FORM,
            INVALID_ABSOLUTE_AUTHORITY,
            MISSING_HOST,
            INVALID_HOST,
            AMBIGUOUS_FRAMING,
            INVALID_CONTENT_LENGTH,
            COUNT
        };

        explicit RequestParseError(Id id);

        int getErrorCode() const { return _errorCode; }
};
