/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 00:00:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/15 00:00:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <headers.hpp>
#include "../../../support/test_assertions.hpp"

// =====================================================================
// 	CASE-INSENSITIVE LOOKUP AND REPEATED FIELDS
// =====================================================================
static void repeatedFields(TestSuite& suite)
{
    Headers headers;
    suite.check(!headers.has("missing"), "missing field is absent");
    suite.check(headers.get("missing").empty(), "missing single value is empty");
    suite.check(headers.getAll("missing").empty(), "missing field has no values");
    headers.add("X-Empty", "");
    suite.check(headers.has("x-empty"), "empty field is present");
    suite.check(headers.getAll("X-EMPTY").size() == 1, "empty field has one value");
    suite.check(headers.getAll("x-empty")[0].empty(), "empty value is retained");

    headers.add("Host", "example.com");
    suite.checkEqual(headers.get("HOST"), std::string("example.com"), "uppercase lookup");
    suite.checkEqual(headers.get("host"), std::string("example.com"), "lowercase lookup");
    suite.checkEqual(headers.get("hOsT"), std::string("example.com"), "mixed-case lookup");
    headers.add("X-Repeat", "first");
    headers.add("Other", "between");
    headers.add("x-repeat", "second");
    std::vector<std::string> repeated = headers.getAll("X-REPEAT");
    suite.check(repeated.size() == 2, "repeated field is not overwritten");
    if (repeated.size() == 2)
    {
        suite.checkEqual(repeated[0], std::string("first"), "first repeated value");
        suite.checkEqual(repeated[1], std::string("second"), "second repeated value preserves arrival order");
    }
    suite.checkEqual(headers.get("x-repeat"), std::string("first"), "single-value access returns the first value");

    const char* names[] = {"X-Empty", "Host", "X-Repeat", "Other", "x-repeat"};
    size_t i = 0;
    for (Headers::const_iterator it = headers.begin(); it != headers.end(); ++it, ++i)
        suite.check(i < 5 && it->first == names[i], "iteration preserves field spelling and wire order");
    suite.check(i == 5, "iteration includes every repeated field");

    headers["X-repeat"] = "changed";
    repeated = headers.getAll("x-repeat");
    suite.check(repeated.size() == 2, "explicit assignment retains the other occurrences");
    if (repeated.size() == 2)
    {
        suite.checkEqual(repeated[0], std::string("changed"), "explicit assignment changes the first value");
        suite.checkEqual(repeated[1], std::string("second"), "explicit assignment preserves the second value");
    }
    headers.remove("X-REPEAT");
    suite.check(!headers.has("x-repeat"), "remove deletes every case variant");
    suite.check(headers.has("other"), "remove preserves unrelated fields");

    Headers aliased;
    aliased.add("Host", "one");
    aliased.add("host", "two");
    aliased.add("Other", "three");
    aliased.remove(aliased.begin()->first);
    suite.check(!aliased.has("HOST") && aliased.has("other"), "remove accepts a key referring to its own storage");
}

// =====================================================================
// 	COPYING AND COMPARISON
// =====================================================================
static void copyAndCompare(TestSuite& suite)
{
    Headers first;
    first.add("A", "one");
    first.add("B", "two");
    first.add("a", "three");
    Headers reordered;
    reordered.add("b", "two");
    reordered.add("a", "one");
    reordered.add("A", "three");
    suite.check(first == reordered, "name case and order between different names do not affect equality");
    Headers reversed;
    reversed.add("a", "three");
    reversed.add("a", "one");
    reversed.add("b", "two");
    suite.check(!(first == reversed), "repeated value order affects equality");
    Headers copy(first);
    copy["b"] = "TWO";
    suite.check(!(first == copy), "field values remain case-sensitive");
    suite.checkEqual(first.get("b"), std::string("two"), "copy owns its values");
    Headers assigned;
    assigned = first;
    assigned = assigned;
    suite.check(assigned == first, "assignment and self-assignment preserve all fields");
    assigned.add("Extra", "");
    suite.check(!(assigned == first), "extra empty fields affect equality");

    std::map<std::string, std::string> values;
    values["Host"] = "one";
    values["host"] = "two";
    const Headers fromMap(values);
    suite.check(fromMap.getAll("HOST").size() == 2, "map construction retains differently spelled repeated names");
    Headers changed(first);
    const std::vector<std::string> snapshot = changed.getAll("A");
    changed.remove("A");
    suite.check(snapshot.size() == 2, "getAll returns an independent snapshot");
}

int main()
{
    TestSuite suite("HTTP header storage");
    repeatedFields(suite);
    copyAndCompare(suite);
    return suite.finish();
}
