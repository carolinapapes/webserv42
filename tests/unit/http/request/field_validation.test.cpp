/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_validation.test.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 00:00:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/15 00:00:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../support/http_request_test.hpp"
#include <field_validators.hpp>

static void expectParse(TestSuite& suite, const std::string& name,
                         const std::string& line, const std::string& fields,
                         int errorCode, const std::string& body = "")
{
    const RequestParseResult result = parseRequest(line + "\r\n" + fields + "\r\n" + body, true);
    suite.checkEqual(result.getErrorCode(), errorCode, name + " HTTP error");
    suite.checkEqual(result.status, errorCode == 0 ? RequestParseResult::COMPLETE : RequestParseResult::ERROR,
                     name + " outcome");
    suite.checkEqual(result.message.empty(), errorCode == 0, name + " diagnostic");
    if (errorCode == 0)
        suite.checkEqual(result.request.getBody(), body, name + " body remains unchanged");
}

// =====================================================================
// 	HEADER SYNTAX AND HOST
// =====================================================================
static void headerValidation(TestSuite& suite)
{
    struct HeaderCase { const char* fields; int code; };
    const HeaderCase cases[] = {
        {"host: example.com\r\n", 0},
        {"HOST: example.com\r\n", 0},
        {"HoSt: \texample.com\t \r\n", 0},
        {"Host: example.com\r\nX-Empty:\r\n", 0},
        {"Host: example.com\r\nX!#$%&'*+-.^_`|~: token\r\n", 0},
        {"Host: example.com\r\nX-Test: one\ttwo\r\n", 0},
        {"Host: example.com\r\nHost: example.com\r\n", 400},
        {"Host: example.com\r\nhOsT: other.example\r\n", 400},
        {"", 400},
        {"Host:\r\n", 400},
        {"Host: example.com:8080\r\n", 0},
        {"Host: 127.0.0.1:65535\r\n", 0},
        {"Host: example.com:0\r\n", 0},
        {"Host: [::1]\r\n", 0},
        {"Host: [2001:db8::1]:443\r\n", 0},
        {"Host: [::ffff:192.0.2.1]:80\r\n", 0},
        {"Host: [v1.future:host]\r\n", 0},
        {"Host: ex%61mple.com\r\n", 0},
        {"Host: example.com%%\r\n", 400},
        {"Host: user@example.com\r\n", 400},
        {"Host: example.com/path\r\n", 400},
        {"Host: example.com?query\r\n", 400},
        {"Host: example.com#fragment\r\n", 400},
        {"Host: example com\r\n", 400},
        {"Host: example.com:\r\n", 400},
        {"Host: example.com:+80\r\n", 400},
        {"Host: example.com:-1\r\n", 400},
        {"Host: example.com:65536\r\n", 400},
        {"Host: example.com:999999999999999999999999\r\n", 400},
        {"Host: ::1\r\n", 400},
        {"Host: []\r\n", 400},
        {"Host: [::g]\r\n", 400},
        {"Host: [::1\r\n", 400},
        {"Host: [::1]extra\r\n", 400},
        {"Host: [::1]:\r\n", 400},
        {"Host: [v.future]\r\n", 400},
        {"Host: example.com\r\n folded: value\r\n", 400},
        {"Host: example.com\r\n\tfolded\r\n", 400},
        {"Host : example.com\r\n", 400},
        {"Host: example.com\r\nX-Test: \vvalue\r\n", 400},
        {"Host: example.com\r\nX-Test: value\f\r\n", 400},
        {"Host: example.com\r\nContent-Length: 0\r\ncontent-length: 00\r\n", 0},
        {"Host: example.com\r\nContent-Length: 0\r\ncontent-length: 1\r\n", 400},
        {"Host: example.com\r\ncontent-length: bad\r\n", 400},
        {"Host: example.com\r\nContent-Length: 2147483648\r\n", 400},
        {"Host: example.com\r\nContent-Length: 0, 0\r\n", 400},
        {"Host: example.com\r\nContent-Length: 0\r\nTransfer-Encoding: chunked\r\n", 400},
        {"Host: example.com\r\ntransfer-encoding: chunked\r\nCONTENT-LENGTH: 0\r\n", 400}
    };
    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
    {
        std::ostringstream name;
        name << "header case " << i;
        expectParse(suite, name.str(), "GET / HTTP/1.1", cases[i].fields, cases[i].code);
    }
    const unsigned char controls[] = {0, 1, 8, 11, 12, 31, 127};
    for (size_t i = 0; i < sizeof(controls) / sizeof(controls[0]); ++i)
    {
        const std::string value(1, static_cast<char>(controls[i]));
        suite.check(!isValidValue(value), "invalid control byte is rejected");
        expectParse(suite, "control byte in field", "GET / HTTP/1.1",
                     "Host: example.com\r\nX-Test: " + value + "\r\n", 400);
    }
    const std::string opaque = std::string(1, static_cast<char>(128)) + static_cast<char>(255);
    suite.check(isValidValue(opaque), "opaque high bytes are valid generic field content");
    expectParse(suite, "opaque field content", "GET / HTTP/1.1", "Host: example.com\r\nX-Test: " + opaque + "\r\n", 0);
    suite.check(isValidKey(std::string(256, 'a')), "existing field-name boundary remains valid");
    suite.check(!isValidKey(std::string(257, 'a')), "oversized field name remains invalid");
    suite.check(isValidValue(std::string(8192, 'a')), "existing field-value boundary remains valid");
    suite.check(!isValidValue(std::string(8193, 'a')), "oversized field value remains invalid");
    suite.check(!isValidKey(std::string(1, static_cast<char>(255))), "field names remain ASCII tokens");
}

// =====================================================================
// 	CONTENT-LENGTH AND TRANSFER-ENCODING
// =====================================================================
static void framingHeaders(TestSuite& suite)
{
    struct LengthCase { const char* value; bool valid; size_t length; };
    const LengthCase lengths[] = {
        {"0", true, 0}, {"3", true, 3}, {"0003", true, 3},
        {"2147483647", true, 2147483647}, {"2147483648", false, 0},
        {"999999999999999999999999999999", false, 0}, {"", false, 0},
        {"-1", false, 0}, {"+1", false, 0}, {"1x", false, 0},
        {"1.0", false, 0}, {" 1", false, 0}, {"1 ", false, 0}, {"1 0", false, 0}
    };
    for (size_t i = 0; i < sizeof(lengths) / sizeof(lengths[0]); ++i)
    {
        size_t value = 42;
        suite.checkEqual(parseContentLength(lengths[i].value, value), lengths[i].valid, "Content-Length numeric validity");
        suite.checkEqual(value, lengths[i].length, "Content-Length numeric result or reset on failure");
    }
    struct TransferCase { const char* value; int code; };
    const TransferCase transfers[] = {
        {"chunked", 0}, {"CHUNKED", 0}, {" \tchunked\t ", 0},
        {"", 400}, {"gzip", 400}, {"chunked, gzip", 400},
        {"chunked, chunked", 400}, {"chunked;foo=bar", 400},
        {",chunked", 0}, {"chunked,", 0}, {"gzip,,chunked", 501},
        {", ,", 400}, {" , chunked , , ", 0},
        {"\r\nTransfer-Encoding: chunked", 0},
        {"gzip, chunked", 501}, {"custom; level=1, chunked", 501},
        {"custom; p=\"a,b\", chunked", 501},
        {"custom; p=\"a\\\"b\", chunked", 501},
        {"custom; p=\"unterminated, chunked", 400},
        {"custom; p, chunked", 400}, {"custom; p=, chunked", 400},
        {"chunked\r\nTransfer-Encoding: chunked", 400},
        {"gzip\r\ntransfer-encoding: chunked", 501}
    };
    for (size_t i = 0; i < sizeof(transfers) / sizeof(transfers[0]); ++i)
    {
        std::ostringstream name;
        name << "Transfer-Encoding case " << i;
        expectParse(suite, name.str(), "POST / HTTP/1.1",
                     std::string("Host: example.com\r\nTransfer-Encoding: ") + transfers[i].value + "\r\n",
                     transfers[i].code, transfers[i].code == 0 ? "0\r\n\r\n" : "");
    }
}

// =====================================================================
// 	REQUEST LINE AND TARGET FORMS
// =====================================================================
static void requestLines(TestSuite& suite)
{
    struct LineCase { const char* line; int code; };
    const LineCase lines[] = {
        {"GET / HTTP/1.1", 0}, {"POST / HTTP/1.1", 0},
        {"PUT / HTTP/1.1", 0}, {"DELETE / HTTP/1.1", 0},
        {"FOO / HTTP/1.1", 501}, {"HEAD / HTTP/1.1", 501},
        {"get / HTTP/1.1", 501}, {"X!Method / HTTP/1.1", 501},
        {"GE:T / HTTP/1.1", 400}, {"GE(T / HTTP/1.1", 400},
        {"GE/T / HTTP/1.1", 400}, {"GET\t/ HTTP/1.1", 400},
        {"GET / HTTP/2.0", 505}, {"GET / HTTP/1.0", 505}, {"GET / HTTP/1.2", 505},
        {"GET / HTTP/1.", 400}, {"GET / HTTP/1.11", 400}, {"GET / Http/1.1", 400},
        {"GET / HTTP/a.b", 400}, {"GET / HTTP/1.1 extra", 400},
        {"GET /hello%20world?q=%2f HTTP/1.1", 0},
        {"GET //double//slash HTTP/1.1", 0},
        {"GET /p?q=a/b?c:d@e&x='yes' HTTP/1.1", 0},
        {"GET http://example.com/path?q=%20 HTTP/1.1", 0},
        {"GET https://[::1]:443/path HTTP/1.1", 0},
        {"GET HTTP://example.com HTTP/1.1", 0},
        {"GET http://example.com?query HTTP/1.1", 0},
        {"GET index.html HTTP/1.1", 400}, {"GET /p#fragment HTTP/1.1", 400},
        {"GET /p?q=%GG HTTP/1.1", 400}, {"GET /p?q=% HTTP/1.1", 400},
        {"GET /p?q=[bad] HTTP/1.1", 400}, {"GET /bad\\path HTTP/1.1", 400},
        {"GET http:///path HTTP/1.1", 400}, {"GET http:// HTTP/1.1", 400},
        {"GET http://user@example.com/ HTTP/1.1", 400},
        {"GET http://example.com:/ HTTP/1.1", 400},
        {"GET http://[::1 HTTP/1.1", 400},
        {"GET http://example.com:99999/ HTTP/1.1", 400},
        {"GET ftp://example.com/path HTTP/1.1", 400},
        {"CONNECT example.com:443 HTTP/1.1", 501},
        {"CONNECT [::1]:443 HTTP/1.1", 501},
        {"CONNECT / HTTP/1.1", 400}, {"CONNECT example.com HTTP/1.1", 400},
        {"GET example.com:443 HTTP/1.1", 400},
        {"OPTIONS * HTTP/1.1", 501}, {"GET * HTTP/1.1", 400}
    };
    for (size_t i = 0; i < sizeof(lines) / sizeof(lines[0]); ++i)
    {
        std::ostringstream name;
        name << "request line case " << i;
        expectParse(suite, name.str(), lines[i].line, "Host: example.com\r\n", lines[i].code);
    }
    suite.check(validMethod("FOO") && !isSupportedMethod("FOO"), "method syntax and implementation support are separate");
    suite.check(!validMethod("GE:T"), "malformed method is not merely unsupported");
}

static void parsedFields(TestSuite& suite)
{
    Headers expected;
    expected.add("host", "example.com");
    expected.add("X!Empty", "");
    expected.add("X-Repeat", "one");
    expected.add("x-repeat", "two");
    expected.add("Content-Length", "3");
    const std::string body("a\0b", 3);
    const std::string raw = std::string("POST /p?q=%2f HTTP/1.1\r\nhost: example.com\r\nX!Empty:\r\nX-Repeat: one\r\nx-repeat: two\r\nContent-Length: 3\r\n\r\n") + body;
    RequestTestCase("complete fields with repeated headers", raw,
        ControlData("POST", "/p?q=%2f", "HTTP/1.1"), expected, body).run(suite);
    const Request actual = validateRequest(raw);
    const std::vector<std::string> values = actual.getHeaders().getAll("X-REPEAT");
    suite.check(values.size() == 2, "parsed repeated field values remain available");
    if (values.size() == 2)
        suite.check(values[0] == "one" && values[1] == "two", "parsed repeated fields retain arrival order");
}

int main()
{
    TestSuite suite("HTTP field validation");
    try
    {
        headerValidation(suite);
        framingHeaders(suite);
        requestLines(suite);
        parsedFields(suite);
    }
    catch (const std::exception& error)
    {
        suite.check(false, std::string("unexpected exception: ") + error.what());
    }
    return suite.finish();
}
