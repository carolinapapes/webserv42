/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   path.test.cpp                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <path.hpp>
#include "../../../support/test_assertions.hpp"

#include <stdexcept>

int main()
{
    TestSuite suite("Path normalization");
    try
    {
        const char *paths[] = {
            "/taha/tahere", "/taha/tahere/", "taha/tahere",
            "../taha//tahere", "pedro/../taha/./tahere"
        };
        for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); ++i)
            suite.check(Path(paths[i]).getPathText() == "/taha/tahere", paths[i]);

        const std::vector<std::string> sections = Path("/taha%2f/tahere").getSections();
        suite.check(sections.size() == 2, "encoded slash stays in its original segment");
        if (sections.size() == 2)
        {
            suite.check(sections[0] == "taha/", "percent-decoded first segment");
            suite.check(sections[1] == "tahere", "second segment");
        }

        suite.check(Path("hola_mundo") == Path("hola%5fmundo"), "encoded and literal underscore compare equal");
        suite.check(!(Path("hola_mundo") == Path("hola__mundo")), "different paths compare unequal");

        const Path child("/taha/tahere/el%20berkani");
        const Path parent("/taha/tahere/");
        const Path unrelated("/mohamed/tahere/");
        const Path descendant("/taha/tahere/el%20berkani/sthhoeunta/stnaoehusn/tohenut/tuh/ooeu/oeu/oeu");
        suite.check(parent.is_prefix_of(child) == 1, "parent has one additional descendant segment");
        suite.check(parent.is_prefix_of(parent) == 0, "same path is a prefix with zero extra segments");
        suite.check(parent.is_prefix_of(unrelated) == -1, "unrelated path is not a prefix");
        suite.check(parent.is_prefix_of(descendant) == 8, "deep descendant segment count");
    }
    catch (const std::exception &error)
    {
        suite.check(false, error.what());
    }
    return suite.finish();
}
