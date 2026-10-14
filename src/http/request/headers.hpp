/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:53:34 by capapes           #+#    #+#             */
/*   Updated: 2025/12/03 17:58:04 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>

class Headers {
public:
    Headers();
    Headers(const std::map<std::string, std::string>& vals);
    Headers(const Headers& other);
    Headers	&operator=(const Headers& other);
    ~Headers();

    typedef std::vector<std::pair<std::string, std::string> > Fields;
    typedef Fields::iterator iterator;
    typedef Fields::const_iterator const_iterator;

    bool operator==(const Headers& other) const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    void 				add(const std::string& key, const std::string& value);
    void 				remove(const std::string& key);

    bool 				has(const std::string& key) const;
    const std::string&	get(const std::string& key) const;
    std::vector<std::string> getAll(const std::string& key) const;
    std::string			&operator[](const std::string& key);

private:
    Fields values;
};
