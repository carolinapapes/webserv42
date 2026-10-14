/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ip_literal.test.cpp                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <ip_literal.hpp>
#include <uri.hpp>
#include "../../../support/test_assertions.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>

int main()
{
    TestSuite suite("IP literals");
    const char *addresses[] = {
        "126.1.0.42", "126.1.77.42", "::", "23a::aab",
        "23a:3:ff:1234::", "23a:3:ff:1234::ca1", "ff1:2:ad3::cd6:7:aaa8"
    };
    for (size_t i = 0; i < sizeof(addresses) / sizeof(addresses[0]); ++i)
    {
        try
        {
            const bool ipv6 = std::strchr(addresses[i], ':') != NULL;
            const int family = ipv6 ? AF_INET6 : AF_INET;
            const size_t bytes = ipv6 ? IP_V6_DATA_SIZE : IP_V4_DATA_SIZE;
            uint8_t expected[IP_MAX_DATA_SIZE] = {0};
            suite.check(inet_pton(family, addresses[i], expected) == 1, "reference address parses");
            const std::string literal = ipv6 ? "[" + std::string(addresses[i]) + "]" : addresses[i];
            IpLiteral ip(literal);
            suite.check(ip.getType() == (ipv6 ? IP_V_6 : IP_V_4), literal + " address family");
            suite.check(std::memcmp(ip.getData(), expected, bytes) == 0, literal + " bytes match inet_pton");

            URI uri("http://" + literal);
            suite.check(std::memcmp(uri.getAuthority().getHost().getIp().getData(), expected, bytes) == 0,
                        literal + " is preserved through URI parsing");
        }
        catch (const std::exception &error)
        {
            suite.check(false, std::string(addresses[i]) + ": " + error.what());
        }
    }
    return suite.finish();
}
