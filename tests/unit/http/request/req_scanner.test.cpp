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
#include "../../../support/test_assertions.hpp"
#include <stdexcept>

// =====================================================================
// 	SEQUENTIAL EXTRACTION
// =====================================================================
static void sequentialExtraction(TestSuite& suite)
{
    std::string field;
    ReqScanner line("GET / HTTP/1.1");
    suite.check(line.getField(" ", field), "method delimiter found");
    suite.checkEqual(field, std::string("GET"), "extract method");
    suite.check(line.getField(" ", field), "target delimiter found");
    suite.checkEqual(field, std::string("/"), "extract request target");
    suite.checkEqual(line.getRemaining(), std::string("HTTP/1.1"), "extract HTTP version explicitly");
    suite.check(line.isEndPos(), "scanner finishes after last field");
    field = "stale";
    suite.check(!line.getField(" ", field), "reading after end needs more input");
    suite.check(field.empty(), "reading after end clears output");
    suite.check(line.getRemaining().empty(), "repeated remainder is empty");

    ReqScanner empty("");
    suite.check(empty.isEndPos(), "empty scanner starts at end");
    suite.check(!empty.getField(":", field), "empty input has no delimiter");
    suite.check(field.empty(), "empty input clears output");
    suite.check(empty.getRemaining().empty(), "empty input has an empty remainder");

    ReqScanner adjacent("key::value");
    suite.check(adjacent.getField(":", field) && field == "key", "extract key");
    suite.check(adjacent.getField(":", field), "adjacent delimiter is successful extraction");
    suite.check(field.empty(), "adjacent delimiter yields an empty field");
    suite.checkEqual(adjacent.getRemaining(), std::string("value"), "continue after adjacent delimiter");

    ReqScanner trailing("value:");
    suite.check(trailing.getField(":", field) && field == "value", "delimiter at the end");
    suite.check(trailing.isEndPos(), "trailing delimiter consumes exactly the input");

    ReqScanner leading(":value");
    suite.check(leading.getField(":", field) && field.empty(), "leading delimiter extracts an empty field");
    suite.checkEqual(leading.getRemaining(), std::string("value"), "leading delimiter advances exactly once");
}

// =====================================================================
// 	MISSING DELIMITERS
// =====================================================================
static void missingDelimiters(TestSuite& suite)
{
    const char* delimiters[] = {"|", "::", "\r\n\r\n", "longer-than-input"};
    for (size_t i = 0; i < sizeof(delimiters) / sizeof(delimiters[0]); ++i)
    {
        ReqScanner scanner("abcdef");
        std::string field("stale");
        suite.check(!scanner.getField(delimiters[i], field), "missing delimiter reports incomplete extraction");
        suite.check(field.empty(), "missing delimiter clears stale output");
        suite.check(!scanner.isEndPos(), "missing delimiter does not mark the input consumed");
        suite.check(!scanner.getField(delimiters[i], field), "repeated missing delimiter remains incomplete");
        suite.checkEqual(scanner.getRemaining(), std::string("abcdef"), "missing delimiter never advances the position");
    }

    std::string field;
    ReqScanner middle("GET / HTTP/1.1");
    suite.check(middle.getField(" ", field) && field == "GET", "consume a field before a failed extraction");
    suite.check(!middle.getField("::", field), "delimiter missing at a nonzero position");
    suite.check(middle.getField(" ", field) && field == "/", "a failed extraction does not skip subsequent input");
    suite.checkEqual(middle.getRemaining(), std::string("HTTP/1.1"), "remaining version is intact after retry");

    ReqScanner partial("line\r");
    suite.check(!partial.getField("\r\n", field), "partial CRLF needs more input");
    suite.checkEqual(partial.getRemaining(), std::string("line\r"), "partial delimiter bytes are preserved");

    ReqScanner invalidDelimiter("key:value");
    bool threw = false;
    try { invalidDelimiter.getField("", field); }
    catch (const std::invalid_argument&) { threw = true; }
    suite.check(threw, "empty delimiter is rejected; it is not an EOF convention");
    suite.checkEqual(invalidDelimiter.getRemaining(), std::string("key:value"), "invalid delimiter does not move the position");
}

// =====================================================================
// 	INPUT OWNERSHIP AND INDEPENDENCE
// =====================================================================
static void independentScanners(TestSuite& suite)
{
    std::string source("one:two");
    ReqScanner first(source);
    source[0] = 'X';
    ReqScanner second("other:value");
    std::string field;
    suite.check(first.getField(":", field) && field == "one", "scanner owns a copy of its input");
    ReqScanner copied(first);
    suite.check(second.getField(":", field) && field == "other", "a second scanner has independent input");
    suite.checkEqual(copied.getRemaining(), std::string("two"), "copied scanner owns its position");
    suite.checkEqual(first.getRemaining(), std::string("two"), "advancing a copy does not change the original");
    suite.checkEqual(second.getRemaining(), std::string("value"), "advancing another scanner does not change the second");

    ReqScanner binary(std::string("a\0b", 3));
    suite.check(binary.getField(std::string(1, '\0'), field) && field == "a", "NUL is a real delimiter when explicitly supplied");
    suite.checkEqual(binary.getRemaining(), std::string("b"), "continue after a binary delimiter");
    ReqScanner binaryRemainder(std::string("a\0b", 3));
    suite.checkEqual(binaryRemainder.getRemaining(), std::string("a\0b", 3), "remainder preserves embedded NUL bytes");

    ReqScanner aliased("key::value");
    std::string delimiter("::");
    suite.check(aliased.getField(delimiter, delimiter) && delimiter == "key",
                "delimiter and output may refer to the same string");
    suite.checkEqual(aliased.getRemaining(), std::string("value"),
                     "aliased output does not change the consumed delimiter length");
}

int main()
{
    TestSuite suite("Request scanner");
    sequentialExtraction(suite);
    missingDelimiters(suite);
    independentScanners(suite);
    return suite.finish();
}
