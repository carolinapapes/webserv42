/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assertions.test.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 23:00:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/14 23:00:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../support/http_request_test.hpp"

static Request expectedRequest()
{
    Headers headers;
    headers["Host"] = "example.com";
    headers["Content-Length"] = "3";
    return Request(ControlData("POST", "/submit", "HTTP/1.1"), headers, "abc");
}

static Request changedRequest(const Request &expected, const std::string &field)
{
    Request actual(expected);
    ControlData control = actual.getControlData();
    Headers headers = actual.getHeaders();
    if (field == "status") actual.setErrorCode(400);
    if (field == "method") control.method = "GET";
    if (field == "target") control.requestTarget = "/wrong";
    if (field == "version") control.httpVersion = "HTTP/9.9";
    if (field == "headers") headers["Host"] = "wrong.example";
    if (field == "missing-header") headers.remove("Host");
    if (field == "extra-header") headers["X-Unexpected"] = "value";
    if (field == "body") actual.setBody("xyz");
    actual.setControlData(control);
    actual.setHeaders(headers);
    return actual;
}

int main(int argc, char **argv)
{
    const Request expected = expectedRequest();
    // The runner self-test launches each deliberate failure as a subprocess.
    if (argc == 2)
    {
        TestSuite failure("Deliberate mismatch");
        checkRequest(failure, argv[1], changedRequest(expected, argv[1]), expected);
        return failure.finish();
    }

    TestSuite suite("HTTP assertions");
    checkRequest(suite, "matching request", expected, expected);
    const char *fields[] = {"status", "method", "target", "version", "headers",
                            "missing-header", "extra-header", "body"};
    for (size_t i = 0; i < sizeof(fields) / sizeof(fields[0]); ++i)
    {
        std::ostringstream diagnostics;
        std::streambuf *previousError = std::cerr.rdbuf(diagnostics.rdbuf());
        std::streambuf *previousOutput = std::cout.rdbuf(diagnostics.rdbuf());
        TestSuite failure("Expected mismatch");
        checkRequest(failure, fields[i], changedRequest(expected, fields[i]), expected);
        const int status = failure.finish();
        std::cerr.rdbuf(previousError);
        std::cout.rdbuf(previousOutput);
        suite.check(failure.failureCount() == 1, std::string(fields[i]) + " mismatch detected exactly once");
        suite.check(status != 0, std::string(fields[i]) + " mismatch returns failure");
        suite.check(diagnostics.str().find(fields[i]) != std::string::npos,
                    std::string(fields[i]) + " diagnostic identifies the case");
    }
    return suite.finish();
}
