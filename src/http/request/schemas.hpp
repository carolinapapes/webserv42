/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schemas.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:02:42 by capapes           #+#    #+#             */
/*   Updated: 2025/12/03 18:01:04 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "request.hpp"

// =====================================================================
// 	Conditional Schemas flags
// =====================================================================
enum SchemaFlags {
    NONE            = 0,
    LEADING         = 1 << 0,
    TRAILING        = 1 << 1,
    OPTIONAL_SPACES = (LEADING | TRAILING),
    IS_REQUIRED     = 1 << 2,
};

// =====================================================================
// 	DELIMITERS DEFINITIONS
// =====================================================================
#define SP				" "
#define TB				"\t"
#define END_OF_HEADERS 	"\r\n\r\n"
#define END_OF_LINE 	"\r\n"
#define NEW_LINE        "\n"
#define EMPTY_STRING    ""
#define SPACES          " \t\n\r\f\v"

// =====================================================================
// 	SCHEMA DEFINITIONS
// =====================================================================
struct SchemaItem {
    bool 			(*fn)(const std::string&);
    const char* 	error;
    int 			errorCode;
    int             flags; // Flags for optional spaces, allow empty, etc.
};

// =====================================================================
// 	PARSING RESULTS
// =====================================================================
struct RequestParseResult {
    enum Status { COMPLETE, INCOMPLETE, ERROR };

    Status      status;
    Request     request;
    std::string message;

    RequestParseResult() : status(INCOMPLETE) {}
    int getErrorCode() const { return request.getErrorCode(); }
};

// A false endOfInput allows an unfinished request line or headers to be retried
// with more bytes. Body framing is introduced separately in issue #5.
RequestParseResult parseRequest(const std::string& raw, bool endOfInput = false);

// Compatibility entry point for callers supplying a final, whole buffer.
Request validateRequest(const std::string& raw);
