/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   known_bugs.test.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 23:00:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/14 23:00:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../support/http_request_test.hpp"
#include <field_validators.hpp>

int main()
{
    TestSuite suite("HTTP known bugs");
    // These are desired results, not assertions of today's broken behavior.
    // See docs/testing_http.md for the reproductions and their scope.
    RequestTestCase("missing Host with another header",
        "GET / HTTP/1.1\r\nUser-Agent: TestAgent\r\n\r\n", 400).run(suite);
    RequestTestCase("duplicate Host",
        "GET / HTTP/1.1\r\nHost: example.com\r\nHost: other.example\r\n\r\n", 400).run(suite);
    RequestTestCase("conflicting duplicate Content-Length",
        "POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: 3\r\nContent-Length: 4\r\n\r\nabcd", 400).run(suite);
    RequestTestCase("mixed-case malformed Content-Length",
        "POST / HTTP/1.1\r\nHost: example.com\r\ncOnTeNt-LeNgTh: nope\r\n\r\n", 400).run(suite);
    RequestTestCase("malformed escape in query target",
        "GET /search?q=%GG HTTP/1.1\r\nHost: example.com\r\n\r\n", 400).run(suite);
    RequestTestCase("extra request-line token",
        "GET / HTTP/1.1 extra\r\nHost: example.com\r\n\r\n", 400).run(suite);

    Headers headers;
    headers["host"] = "example.com";
    RequestTestCase("lowercase Host name",
        "GET / HTTP/1.1\r\nhost: example.com\r\n\r\n",
        ControlData("GET", "/", "HTTP/1.1"), headers, "").run(suite);

    headers.remove("host");
    headers["Host"] = "example.com";
    headers["X-Empty"] = "";
    RequestTestCase("empty optional field value",
        "GET / HTTP/1.1\r\nHost: example.com\r\nX-Empty:\r\n\r\n",
        ControlData("GET", "/", "HTTP/1.1"), headers, "").run(suite);

    headers.remove("X-Empty");
    headers["X!Trace"] = "token";
    RequestTestCase("valid token punctuation in field name",
        "GET / HTTP/1.1\r\nHost: example.com\r\nX!Trace: token\r\n\r\n",
        ControlData("GET", "/", "HTTP/1.1"), headers, "").run(suite);

    // The existing numeric validator declares an INT_MAX bound. Exercise it
    // directly, without creating an incomplete request with a huge body length.
    suite.checkEqual(isValidContentLength("2147483648"), false, "Content-Length exceeds validator INT_MAX");
    suite.checkEqual(isValidContentLength("999999999999999999999999999999"), false,
                     "Content-Length exceeds strtol range");
    return suite.finish();
}
