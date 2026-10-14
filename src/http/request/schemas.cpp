/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schemas.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:03:35 by capapes           #+#    #+#             */
/*   Updated: 2025/12/03 18:03:45 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "field_validators.hpp"
#include "schemas.hpp"
#include "req_scanner.hpp"

namespace
{
// =====================================================================
//  Flags utility functions
// =====================================================================
std::string remove_trailing(const std::string& str, const std::string& chars = SPACES)
{
    const size_t end = str.find_last_not_of(chars);
    if (end == std::string::npos)
        return "";
    return str.substr(0, end + 1);
}

std::string remove_leading(const std::string& str, const std::string& chars = SPACES)
{
    const size_t start = str.find_first_not_of(chars);
    if (start == std::string::npos)
        return "";
    return str.substr(start);
}

std::string applyFlags(std::string str, int flags)
{
    if (flags & LEADING)
        str = remove_leading(str);
    if (flags & TRAILING)
        str = remove_trailing(str);
    return str;
}

// =====================================================================
// 	Common schema functions
// =====================================================================
std::string validateField(const std::string& raw, const SchemaItem& item)
{
    const std::string value = applyFlags(raw, item.flags);
    if ((item.flags & IS_REQUIRED) && value.empty())
        throw RequestParseError(400, item.error);
    if (item.fn && !item.fn(value))
        throw RequestParseError(item.errorCode, item.error);
    return value;
}

bool readLine(ReqScanner& scanner, std::string& line, bool endOfInput)
{
    if (scanner.getField(END_OF_LINE, line))
    {
        if (line.find_first_of("\r\n") != std::string::npos)
            throw RequestParseError(400, "Invalid line ending");
        return true;
    }

    // A final CR can be the start of a fragmented CRLF. Other embedded line
    // endings cannot become valid by appending more input.
    const std::string remaining = scanner.getRemaining();
    const size_t carriageReturn = remaining.find('\r');
    if (remaining.find('\n') != std::string::npos
        || (carriageReturn != std::string::npos && carriageReturn + 1 != remaining.size()))
        throw RequestParseError(400, "Invalid line ending");
    if (endOfInput)
        throw RequestParseError(400, "Unexpected end of request headers");
    return false;
}

// =====================================================================
// 	SCHEMAS
// =====================================================================
static const SchemaItem controlDataItems[] = {
    { validMethod, "Invalid HTTP method", 501, IS_REQUIRED },
    { isValidRequest, "Invalid request target", 400, IS_REQUIRED },
    { isValidProtocol, "Invalid HTTP version", 505, IS_REQUIRED }
};

static const SchemaItem headersItems[] = {
    { isValidKey, "Invalid header key", 400, IS_REQUIRED },
    { isValidValue, "Invalid header value", 400, IS_REQUIRED | OPTIONAL_SPACES }
};

// =====================================================================
// 	STRATEGY VALIDATORS
// =====================================================================
//      Request
//          |-> ControlData
//          |-> HeadersFields [Iterator] -> HeadersItems
//          |-> Body
// =====================================================================
ControlData validateControlData(const std::string& raw)
{
    ReqScanner scanner(raw);
    std::string method;
    std::string target;
    if (!scanner.getField(SP, method) || !scanner.getField(SP, target))
        throw RequestParseError(400, "Invalid request line");

    ControlData result;
    result.method = validateField(method, controlDataItems[0]);
    result.requestTarget = validateField(target, controlDataItems[1]);
    result.httpVersion = validateField(scanner.getRemaining(), controlDataItems[2]);
    specificControlDataValidation(result.requestTarget);
    return result;
}

void validateHeader(const std::string& line, Headers& headers)
{
    ReqScanner scanner(line);
    std::string key;
    if (!scanner.getField(":", key))
        throw RequestParseError(400, "Missing header colon");
    key = validateField(key, headersItems[0]);
    const std::string value = validateField(scanner.getRemaining(), headersItems[1]);
    headers.add(key, value);
}
}

// =====================================================================
// 	REQUEST PARSING
// =====================================================================
RequestParseResult parseRequest(const std::string& raw, bool endOfInput)
{
    RequestParseResult result;
    ReqScanner scanner(raw);
    try
    {
        std::string line;
        if (!readLine(scanner, line, endOfInput))
            return result;
        result.request.setControlData(validateControlData(line));

        Headers headers;
        while (true)
        {
            if (!readLine(scanner, line, endOfInput))
                return result;
            if (line.empty())
                break;
            validateHeader(line, headers);
        }
        specificHeadersValidation(headers);
        result.request.setHeaders(headers);
        result.request.setBody(scanner.getRemaining());
        result.status = RequestParseResult::COMPLETE;
    }
    catch (const RequestParseError& error)
    {
        result.status = RequestParseResult::ERROR;
        result.request.setErrorCode(error.getErrorCode());
        result.message = error.what();
    }
    return result;
}

Request validateRequest(const std::string& raw)
{
    return parseRequest(raw, true).request;
}
