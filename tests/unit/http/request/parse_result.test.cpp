/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_result.test.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 23:50:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/14 23:50:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../support/http_request_test.hpp"
#include <field_validators.hpp>

static void checkResult(TestSuite& suite, const std::string& name,
                         const RequestParseResult& result,
                         RequestParseResult::Status status, int errorCode)
{
    suite.checkEqual(result.status, status, name + " outcome");
    suite.checkEqual(result.getErrorCode(), errorCode, name + " HTTP error");
    suite.checkEqual(result.message.empty(), status != RequestParseResult::ERROR,
                     name + " message is present only for errors");
}

// =====================================================================
// 	INCOMPLETE INPUT AND EXPLICIT END OF INPUT
// =====================================================================
static void incompleteInput(TestSuite& suite)
{
    const std::string raw("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
    for (size_t i = 0; i < raw.size(); ++i)
    {
        std::ostringstream name;
        name << "valid prefix at byte " << i;
        const std::string prefix = raw.substr(0, i);
        checkResult(suite, name.str(), parseRequest(prefix), RequestParseResult::INCOMPLETE, 0);
        checkResult(suite, name.str() + " at end of input", parseRequest(prefix, true),
                     RequestParseResult::ERROR, 400);
        suite.checkEqual(validateRequest(prefix).getErrorCode(), 400,
                         name.str() + " whole-buffer compatibility error");
    }
    const RequestParseResult complete = parseRequest(raw);
    checkResult(suite, "complete request", complete, RequestParseResult::COMPLETE, 0);
    Headers headers;
    headers["Host"] = "example.com";
    const Request expected(ControlData("GET", "/", "HTTP/1.1"), headers, "");
    checkRequest(suite, "explicit result fields", complete.request, expected);
    checkRequest(suite, "whole-buffer compatibility fields", validateRequest(raw), expected);
}

// =====================================================================
// 	ERROR PROPAGATION
// =====================================================================
static void malformedInput(TestSuite& suite)
{
    struct InvalidCase { const char* raw; int errorCode; };
    const InvalidCase cases[] = {
        {"\r\n", 400},
        {"GET\r\n", 400},
        {"GET /\r\n", 400},
        {" / HTTP/1.1\r\n", 400},
        {"GET  HTTP/1.1\r\n", 400},
        {"GET / \r\n", 400},
        {"FOO / HTTP/1.1\r\n", 501},
        {"GET / HTTP/9.9\r\n", 505},
        {"GET /bad% HTTP/1.1\r\n", 400},
        {"GET / HTTP/1.1\r\nHost example.com\r\n", 400},
        {"GET / HTTP/1.1\r\n: value\r\n", 400},
        {"GET / HTTP/1.1\r\nBad Name: value\r\n", 400},
        {"GET / HTTP/1.1\r\nHost:\r\n\r\n", 400},
        {"GET / HTTP/1.1\r\nHost: example.com%%\r\n\r\n", 400},
        {"GET / HTTP/1.1\r\nUser-Agent: TestAgent\r\n\r\n", 400},
        {"GET / HTTP/1.1\r\n\r\n", 400},
        {"POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: nope\r\n\r\n", 400},
        {"GET / HTTP/1.1\n", 400},
        {"GET / HTTP/1.1\rX", 400},
        {"GET / HTTP/1.1\r\r", 400},
        {"GET / HTTP/1.1\r\nHost: example.com\n", 400},
        {"GET / HTTP/1.1\r\nHost: example\r.com\r\n", 400}
    };
    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
    {
        std::ostringstream name;
        name << "malformed case " << i;
        checkResult(suite, name.str(), parseRequest(cases[i].raw),
                     RequestParseResult::ERROR, cases[i].errorCode);
        checkResult(suite, name.str() + " at end of input", parseRequest(cases[i].raw, true),
                     RequestParseResult::ERROR, cases[i].errorCode);
    }

    std::ostringstream diagnostics;
    std::streambuf* previous = std::cerr.rdbuf(diagnostics.rdbuf());
    const RequestParseResult error = parseRequest("GET / HTTP/1.1\r\n\r\n");
    std::cerr.rdbuf(previous);
    suite.check(diagnostics.str().empty(), "parser returns diagnostics instead of writing to stderr");
    suite.checkEqual(error.message, std::string("Missing Host header"), "missing Host retains its explicit reason");
}

// =====================================================================
// 	INDEPENDENT REQUESTS AND VALIDATORS
// =====================================================================
static void independentResults(TestSuite& suite)
{
    const std::string raw = std::string("POST /binary HTTP/1.1\r\nHost: example.com\r\nContent-Length: 3\r\n\r\n")
                            + std::string("a\0b", 3);
    RequestParseResult first = parseRequest(raw);
    checkResult(suite, "binary request", first, RequestParseResult::COMPLETE, 0);
    suite.checkEqual(first.request.getBody(), std::string("a\0b", 3), "explicit result preserves binary body bytes");
    const RequestParseResult copied(first);
    Request unrelated(first.request);
    unrelated.setErrorCode(418);

    const RequestParseResult error = parseRequest("GET /bad% HTTP/1.1\r\n");
    const RequestParseResult partial = parseRequest("GET / HTTP/1.1\r");
    first.request.setBody("changed");
    checkResult(suite, "copied result", copied, RequestParseResult::COMPLETE, 0);
    suite.checkEqual(copied.request.getBody(), std::string("a\0b", 3), "a copy owns its request and body");
    checkResult(suite, "independent error", error, RequestParseResult::ERROR, 400);
    checkResult(suite, "independent partial input", partial, RequestParseResult::INCOMPLETE, 0);
    suite.checkEqual(unrelated.getErrorCode(), 418, "parsing does not mutate another Request");

    bool caught = false;
    try { specificControlDataValidation("/bad%"); }
    catch (const RequestParseError& invalid)
    {
        caught = true;
        suite.checkEqual(invalid.getErrorCode(), 400, "URI validator carries an explicit HTTP error");
    }
    suite.check(caught, "URI validation fails without an active Request");
    Headers noHost;
    caught = false;
    try { specificHeadersValidation(noHost); }
    catch (const RequestParseError& invalid)
    {
        caught = true;
        suite.checkEqual(invalid.getErrorCode(), 400, "header validator carries an explicit HTTP error");
    }
    suite.check(caught, "header validation fails without an active Request");
    suite.checkEqual(unrelated.getErrorCode(), 418, "validators do not mutate another Request");
    const RequestParseResult next = parseRequest(raw);
    checkResult(suite, "parse after an error and incomplete input", next, RequestParseResult::COMPLETE, 0);
    checkRequest(suite, "repeated result", next.request, copied.request);
}

int main()
{
    TestSuite suite("HTTP parse results");
    try
    {
        incompleteInput(suite);
        malformedInput(suite);
        independentResults(suite);
    }
    catch (const std::exception& error)
    {
        suite.check(false, std::string("unexpected exception: ") + error.what());
    }
    return suite.finish();
}
