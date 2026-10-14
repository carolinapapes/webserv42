/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:05:08 by capapes           #+#    #+#             */
/*   Updated: 2025/11/28 20:30:01 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"

namespace
{
bool sameName(const std::string& first, const std::string& second)
{
    if (first.size() != second.size())
        return false;
    for (size_t i = 0; i < first.size(); ++i)
    {
        const char a = first[i] >= 'A' && first[i] <= 'Z' ? first[i] + ('a' - 'A') : first[i];
        const char b = second[i] >= 'A' && second[i] <= 'Z' ? second[i] + ('a' - 'A') : second[i];
        if (a != b)
            return false;
    }
    return true;
}
}

Headers::Headers() {}

Headers::Headers(const std::map<std::string, std::string>& vals)
{
    for (std::map<std::string, std::string>::const_iterator it = vals.begin(); it != vals.end(); ++it)
        add(it->first, it->second);
}

Headers::Headers(const Headers& other) : values(other.values) {}

Headers& Headers::operator=(const Headers& other)
{
    if (this != &other)
        values = other.values;
    return *this;
}

Headers::~Headers() {}

Headers::iterator Headers::begin() { return values.begin(); }
Headers::iterator Headers::end() { return values.end(); }
Headers::const_iterator Headers::begin() const { return values.begin(); }
Headers::const_iterator Headers::end() const { return values.end(); }

bool Headers::operator==(const Headers& other) const
{
    if (values.size() != other.values.size())
        return false;
    for (const_iterator it = begin(); it != end(); ++it)
        if (getAll(it->first) != other.getAll(it->first))
            return false;
    return true;
}

void Headers::add(const std::string& key, const std::string& value)
{
    values.push_back(std::make_pair(key, value));
}

void Headers::remove(const std::string& key)
{
    const std::string name(key);
    for (iterator it = begin(); it != end(); )
    {
        if (sameName(it->first, name))
            it = values.erase(it);
        else
            ++it;
    }
}

bool Headers::has(const std::string& key) const
{
    for (const_iterator it = begin(); it != end(); ++it)
        if (sameName(it->first, key))
            return true;
    return false;
}

const std::string& Headers::get(const std::string& key) const
{
    for (const_iterator it = begin(); it != end(); ++it)
        if (sameName(it->first, key))
            return it->second;
    static const std::string empty;
    return empty;
}

std::vector<std::string> Headers::getAll(const std::string& key) const
{
    std::vector<std::string> result;
    for (const_iterator it = begin(); it != end(); ++it)
        if (sameName(it->first, key))
            result.push_back(it->second);
    return result;
}

std::string& Headers::operator[](const std::string& key)
{
    for (iterator it = begin(); it != end(); ++it)
        if (sameName(it->first, key))
            return it->second;
    add(key, "");
    return values.back().second;
}
