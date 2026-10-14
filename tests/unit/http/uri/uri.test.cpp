/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   uri.test.cpp                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <uri.hpp>
#include "../../../support/test_assertions.hpp"

#include <stdexcept>

int main()
{
    TestSuite suite("URI references");
    try
    {
        URI root("/");
        suite.check(root.getPath().getPathText() == "/", "root path");

        URI full("http://taha:123@datatracker.ietf.org/doc/html/rfc3986#section-3");
        suite.check(full.getScheme() == scheme_HTTP, "HTTP scheme");
        suite.check(full.getAuthority().getHost().getRegName() == "datatracker.ietf.org", "host");
        suite.check(full.getAuthority().getUserInfo().getUser() == "taha", "username");
        suite.check(full.getAuthority().getUserInfo().getPassword() == "123", "password");
        suite.check(full.getPath().getPathText() == "/doc/html/rfc3986", "full URI path");
        suite.check(full.getFragment().getText() == "section-3", "fragment");

        URI withoutUser("http://datatracker.ietf.org/doc/html/rfc3986#section-3");
        suite.check(withoutUser.getAuthority().getUserInfo().getUser().empty(), "absent userinfo has an empty username");

        const char *references[] = {
            "//datatracker.ietf.org/doc/html/rfc3986#section-3",
            "/doc/html/rfc3986#section-3",
            "doc/html/rfc3986#section-3"
        };
        for (size_t i = 0; i < sizeof(references) / sizeof(references[0]); ++i)
        {
            URI uri(references[i]);
            suite.check(uri.getPath().getPathText() == "/doc/html/rfc3986", references[i]);
            suite.check(uri.getFragment().getText() == "section-3", "relative reference fragment");
            bool missingScheme = false;
            try
            {
                uri.getScheme();
            }
            catch (const std::out_of_range &)
            {
                missingScheme = true;
            }
            suite.check(missingScheme, "relative reference has no scheme");
        }

        URI query("https://datatracker.ietf.org/doc/html/rfc3986?user=taha#section-3");
        suite.check(query.getScheme() == scheme_HTTPS, "HTTPS scheme");
        suite.check(query.getQuery().getText() == "user=taha", "query excludes fragment");
        suite.check(query.getFragment().getText() == "section-3", "fragment excludes query");

        UserInfo user("taha@");
        bool missingPassword = false;
        try
        {
            user.getPassword();
        }
        catch (const std::out_of_range &)
        {
            missingPassword = true;
        }
        suite.check(missingPassword, "absent password throws");
    }
    catch (const std::exception &error)
    {
        suite.check(false, error.what());
    }
    return suite.finish();
}
