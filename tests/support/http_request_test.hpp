/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request_test.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 23:00:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/14 23:00:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "test_assertions.hpp"
#include <request.hpp>
#include <schemas.hpp>
#include <exception>
#include <sstream>

inline std::string describeHeaders(const Headers &headers)
{
    std::ostringstream text;
    for (Headers::const_iterator it = headers.begin(); it != headers.end(); ++it)
        text << it->first << ": " << it->second << "; ";
    return text.str();
}

inline void checkRequest(TestSuite &suite, const std::string &name,
                         const Request &actual, const Request &expected)
{
    suite.checkEqual(actual.getErrorCode(), expected.getErrorCode(), name + " error code");
    // Partial state after a parse error is not part of the parser's contract.
    if (expected.getErrorCode() != 0)
        return;
    suite.checkEqual(actual.getControlData().method, expected.getControlData().method, name + " method");
    suite.checkEqual(actual.getControlData().requestTarget, expected.getControlData().requestTarget, name + " target");
    suite.checkEqual(actual.getControlData().httpVersion, expected.getControlData().httpVersion, name + " version");
    suite.check(actual.getHeaders() == expected.getHeaders(), name + " headers (expected ["
                + describeHeaders(expected.getHeaders()) + "], actual ["
                + describeHeaders(actual.getHeaders()) + "])");
    suite.checkEqual(actual.getBody(), expected.getBody(), name + " body");
}

struct RequestTestCase
{
    std::string name;
    std::string raw;
    Request expected;

    RequestTestCase(const std::string &label, const std::string &input, int errorCode)
        : name(label), raw(input), expected()
    {
        expected.setErrorCode(errorCode);
    }

    RequestTestCase(const std::string &label, const std::string &input,
                    const ControlData &control, const Headers &headers,
                    const std::string &body)
        : name(label), raw(input), expected(control, headers, body) {}

    void run(TestSuite &suite) const
    {
        if (expected.getErrorCode() == 0)
        {
            const size_t boundary = raw.find("\r\n\r\n");
            suite.check(boundary != std::string::npos, name + " fixture header terminator");
            if (boundary != std::string::npos)
                suite.checkEqual(raw.substr(boundary + 4), expected.getBody(), name + " fixture body");
            if (expected.getHeaders().has("Content-Length"))
            {
                std::ostringstream length;
                length << expected.getBody().size();
                suite.checkEqual(expected.getHeaders().get("Content-Length"), length.str(),
                                 name + " fixture Content-Length in bytes");
            }
        }
        try
        {
            checkRequest(suite, name, validateRequest(raw), expected);
        }
        catch (const std::exception &error)
        {
            suite.check(false, name + " unexpected exception: " + error.what());
        }
        catch (...)
        {
            suite.check(false, name + " unexpected non-standard exception");
        }
        Request::setActiveRequest(NULL);
    }
};
