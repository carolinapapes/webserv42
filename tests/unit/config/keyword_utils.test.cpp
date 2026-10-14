/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   keyword_utils.test.cpp                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <a_key_word.hpp>
#include "../../support/test_assertions.hpp"

int main()
{
    TestSuite suite("Keyword helpers");
    suite.check(toUpper("HelloWorld") == "HELLOWORLD", "uppercase conversion");
    suite.check(toLower("HelloWorld") == "helloworld", "lowercase conversion");
    suite.check(startsWith("HelloWorld", "Hello"), "matching case-sensitive prefix");
    suite.check(startsWith("HelloWorld", "hello", false), "lowercase case-insensitive prefix");
    suite.check(startsWith("HelloWorld", "HELLO", false), "uppercase case-insensitive prefix");
    suite.check(!startsWith("HelloWorld", "WORLD", true), "non-prefix does not match");
    return suite.finish();
}
