/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   test_assertions.hpp                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <string>

class TestSuite
{
    private:
        std::string _name;
        size_t _checks;
        size_t _failures;

    public:
        explicit TestSuite(const std::string &name)
            : _name(name), _checks(0), _failures(0) {}

        void check(bool condition, const std::string &description)
        {
            ++_checks;
            if (!condition)
            {
                ++_failures;
                std::cerr << _name << ": " << description << std::endl;
            }
        }

        template <typename T>
        void checkEqual(const T &actual, const T &expected,
                        const std::string &description)
        {
            std::ostringstream message;
            message << description << " (expected [" << expected
                    << "], actual [" << actual << "])";
            check(actual == expected, message.str());
        }

        size_t failureCount() const { return _failures; }

        int finish() const
        {
            std::cout << _name << ": " << _checks << " checks, "
                      << _failures << " failures" << std::endl;
            return _failures == 0 ? 0 : 1;
        }
};
