/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   req_scanner.test.cpp                              :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <req_scanner.hpp>
#include <schemas.hpp>
#include "../../../support/test_assertions.hpp"

int main()
{
    TestSuite suite("Request scanner");
    ReqScanner line("GET / HTTP/1.1");
    suite.check(line.getField(SP) == "GET", "extract method");
    suite.check(line.getField(SP) == "/", "extract request target");
    suite.check(line.getField(EOF_) == "HTTP/1.1", "extract HTTP version");
    suite.check(line.isEndPos(), "scanner finishes after last field");
    suite.check(line.getField(SP).empty(), "reading after end returns empty");

    ReqScanner empty("");
    suite.check(empty.isEndPos(), "empty scanner starts at end");
    suite.check(empty.getField(EOF_).empty(), "empty input yields empty field");

    ReqScanner adjacent("key::value");
    suite.check(adjacent.getField(":") == "key", "extract key");
    suite.check(adjacent.getField(":").empty(), "adjacent delimiter yields empty field");
    suite.check(adjacent.getField(EOF_) == "value", "continue after adjacent delimiter");
    return suite.finish();
}
