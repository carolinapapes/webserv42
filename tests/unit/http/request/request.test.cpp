/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:51:48 by capapes           #+#    #+#             */
/*   Updated: 2025/12/03 17:56:54 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../support/http_request_test.hpp"
#include <field_validators.hpp>

#include <vector>

typedef std::vector<RequestTestCase> RequestCases;

// =====================================================================
// 	VALID CASES
// =====================================================================

static RequestCases validCases()
{
    RequestCases cases;
    Headers headers;
    headers["Host"] = "example.com";
    headers["User-Agent"] = "TestAgent";
    headers["Content-Length"] = "0";
    cases.push_back(RequestTestCase("legacy basic GET",
        "GET / HTTP/1.1\r\nHost: example.com\r\nUser-Agent: TestAgent\r\nContent-Length: 0\r\n\r\n",
        ControlData("GET", "/", "HTTP/1.1"), headers, ""));

    headers.remove("User-Agent");
    headers["Content-Type"] = "application/json";
    headers["Content-Length"] = "15";
    cases.push_back(RequestTestCase("legacy JSON POST",
        "POST /submit HTTP/1.1\r\nHost: example.com\r\nContent-Type: application/json\r\nContent-Length: 15\r\n\r\n{\"key\":\"value\"}",
        ControlData("POST", "/submit", "HTTP/1.1"), headers, "{\"key\":\"value\"}"));

    headers.remove("Content-Type");
    headers["Authorization"] = "Bearer token123";
    headers["Content-Length"] = "0";
    cases.push_back(RequestTestCase("legacy authorized PUT",
        "PUT /update HTTP/1.1\r\nHost: example.com\r\nAuthorization: Bearer token123\r\nContent-Length: 0\r\n\r\n",
        ControlData("PUT", "/update", "HTTP/1.1"), headers, ""));

    headers.remove("Authorization");
    cases.push_back(RequestTestCase("legacy query target",
        "GET /search?q=test&lang=en HTTP/1.1\r\nHost: example.com\r\nContent-Length: 0\r\n\r\n",
        ControlData("GET", "/search?q=test&lang=en", "HTTP/1.1"), headers, ""));

    cases.push_back(RequestTestCase("DELETE with empty body",
        "DELETE /resource/1 HTTP/1.1\r\nHost: example.com\r\nContent-Length: 0\r\n\r\n",
        ControlData("DELETE", "/resource/1", "HTTP/1.1"), headers, ""));

    cases.push_back(RequestTestCase("encoded path remains intact",
        "GET /hello%20world HTTP/1.1\r\nHost: example.com\r\nContent-Length: 0\r\n\r\n",
        ControlData("GET", "/hello%20world", "HTTP/1.1"), headers, ""));

    headers.remove("Content-Length");
    cases.push_back(RequestTestCase("Host is sufficient for an empty GET",
        "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n",
        ControlData("GET", "/", "HTTP/1.1"), headers, ""));

    headers["Host"] = "example.com:8080";
    headers["X-Trace"] = "part:one";
    cases.push_back(RequestTestCase("header whitespace and embedded colon",
        "GET / HTTP/1.1\r\nHost: \texample.com:8080 \t\r\nX-Trace: \tpart:one\t \r\n\r\n",
        ControlData("GET", "/", "HTTP/1.1"), headers, ""));

    headers.remove("X-Trace");
    headers["Host"] = "example.com";
    headers["Content-Length"] = "6";
    const std::string binaryBody("a\0b\r\nc", 6);
    cases.push_back(RequestTestCase("binary body preserves NUL and CRLF bytes",
        std::string("POST /binary HTTP/1.1\r\nHost: example.com\r\nContent-Length: 6\r\n\r\n") + binaryBody,
        ControlData("POST", "/binary", "HTTP/1.1"), headers, binaryBody));
    return cases;
}

// =====================================================================
// 	INVALID CASES
// =====================================================================

struct InvalidCase
{
    const char *name;
    const char *raw;
    int errorCode;
};

static const InvalidCase invalidCases[] = {
    // These seven cases, together with the first four valid cases, retain all
    // eleven original scenarios. Malformed requests are labelled explicitly.
    {"legacy malformed Host", "POST /check HTTP/1.1\r\nHost: example.com%%\r\nContent-Length: 0\r\n\r\n", 400},
    {"legacy missing version", "GET /index.html\r\nHost: example.com\r\n\r\n", 400},
    {"legacy missing all headers", "GET /index.html HTTP/1.1\r\n\r\n", 400},
    {"legacy unsupported method", "FOO /test HTTP/1.1\r\nHost: example.com\r\n\r\n", 501},
    {"legacy missing target", "GET  HTTP/1.1\r\nHost: example.com\r\n\r\n", 400},
    {"legacy empty Host", "GET /test HTTP/1.1\r\nHost:\r\n\r\n", 400},
    {"legacy malformed path escape", "GET /hola/%% HTTP/1.1\r\nHost: example.com\r\nContent-Length: 0\r\n\r\n", 400},
    {"empty request", "", 400},
    {"missing request line", "\r\nHost: example.com\r\n\r\n", 400},
    {"unsupported HTTP version", "GET / HTTP/9.9\r\nHost: example.com\r\n\r\n", 505},
    {"missing header terminator", "GET / HTTP/1.1\r\nHost: example.com\r\n", 400},
    {"header without colon", "GET / HTTP/1.1\r\nHost example.com\r\n\r\n", 400},
    {"header without key", "GET / HTTP/1.1\r\nHost: example.com\r\n: value\r\n\r\n", 400},
    {"space in header name", "GET / HTTP/1.1\r\nHost: example.com\r\nBad Name: value\r\n\r\n", 400},
    {"space before header colon", "GET / HTTP/1.1\r\nHost : example.com\r\n\r\n", 400},
    {"control byte in header value", "GET / HTTP/1.1\r\nHost: example.com\r\nX-Test: bad\x01value\r\n\r\n", 400},
    {"empty Content-Length", "POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length:\r\n\r\n", 400},
    {"negative Content-Length", "POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: -1\r\n\r\n", 400},
    {"signed Content-Length", "POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: +1\r\n\r\nx", 400},
    {"alphabetic Content-Length", "POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: abc\r\n\r\n", 400},
    {"Content-Length with trailing junk", "POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: 1x\r\n\r\nx", 400},
    {"fractional Content-Length", "POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: 1.0\r\n\r\nx", 400},
    {"Content-Length with internal space", "POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: 1 0\r\n\r\n", 400},
    {"conflicting Content-Length list", "POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: 1, 2\r\n\r\nx", 400},
    {"truncated path escape", "GET /bad% HTTP/1.1\r\nHost: example.com\r\n\r\n", 400},
    {"short path escape", "GET /bad%2 HTTP/1.1\r\nHost: example.com\r\n\r\n", 400},
    {"non-hexadecimal path escape", "GET /bad%GG HTTP/1.1\r\nHost: example.com\r\n\r\n", 400}
};

// =====================================================================
// 	TEST RUNNER
// =====================================================================

int main()
{
    TestSuite suite("HTTP requests");
    const RequestCases cases = validCases();
    for (RequestCases::const_iterator it = cases.begin(); it != cases.end(); ++it)
        it->run(suite);
    for (size_t i = 0; i < sizeof(invalidCases) / sizeof(invalidCases[0]); ++i)
        RequestTestCase(invalidCases[i].name, invalidCases[i].raw,
                        invalidCases[i].errorCode).run(suite);

    suite.check(isValidContentLength("0"), "zero Content-Length is valid");
    suite.check(isValidContentLength("13"), "positive Content-Length is valid");
    suite.check(isValidContentLength("2147483647"), "validator accepts its INT_MAX boundary on Linux");
    return suite.finish();
}
