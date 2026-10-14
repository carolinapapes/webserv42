/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   query.cpp                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <query.hpp>

#include <string>
#include <vector>
#include <stdexcept>

#include <percent_encoding_utils.hpp>
#include <uri.hpp>

//	class methods.

Query::Query(void)
{
}

Query::Query(std::string text)
{
	if (text[0] != '?')
		throw (std::invalid_argument("no query"));
	this->_text = text.substr(1);
}

Query::Query(std::vector<std::string>::iterator &iter,std::vector<std::string>::iterator end)
{

	if (iter == end || *iter != "?")
		throw (std::invalid_argument("no query"));
	iter++;
	this->_text = "";
	while (iter != end)
	{
		if (*iter == "#" || *iter == "[" || *iter == "]")
			break ;
		this->_text += *iter;
		iter++;
	}
}

Query::~Query(void)
{
}

std::string	Query::getText(void) const
{
	return (this->_text);
}
