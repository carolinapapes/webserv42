/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   uri.cpp                                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <uri.hpp>

#include <string>
#include <stdexcept>
#include <iostream>
#include <path.hpp>
#include <query.hpp>
#include <fragment.hpp>

#include <tokenize.hpp>
#include <scheme.hpp>
#include <authority.hpp>


URI::URI(std::string uri)
{
	this->_isSchemeSet = 0;
	this->_isAuthoritySet = 0;
	this->_isQuerySet = 0;
	this->_isFragmentSet = 0;
	this->_text = uri;

	this->_tokens = tokenize(this->_text, gen_delims);
	if (this->_tokens.size() == 0)
		throw (std::invalid_argument("invalid uri"));

	this->identifyURIType();

	//	parse the each the 5 components of the URI.
	std::vector<std::string>::iterator	iter = this->_tokens.begin();
	std::vector<std::string>::iterator	end = this->_tokens.end();

	//	parse the scheme
	if (this->_type == FULL_FORM)
	{
		this->_isSchemeSet = 1;
		this->_scheme = Scheme(iter, end);
		if (*iter != ":")
			throw (std::invalid_argument("invalid uri"));
		iter++;
	}

	//	parse the autthority
	if (this->_type <= NETWORK_PATH)
	{
		std::vector<std::string>::iterator	check_point = iter;
		try
		{
			this->_authority = Authority(iter, end);
			this->_isAuthoritySet = 1;
		}
		catch (std::exception &e)
		{
			if (std::string("no authority") != std::string(e.what()))
				throw ;
			iter = check_point;
		}
	}

	//	parse the path
	this->_path = Path(iter, end);

	//	parse the query
	{
		std::vector<std::string>::iterator	check_point = iter;
		try
		{
			this->_query = Query(iter, end);
			this->_isQuerySet = 1;
		}
		catch (std::exception &e)
		{
			if (std::string("no query") != std::string(e.what()))
				throw ;
			iter = check_point;
		}
	}

	//	parse the fragment
	{
		std::vector<std::string>::iterator	check_point = iter;
		try
		{
			this->_fragment = Fragment(iter, end);
			this->_isFragmentSet = 1;
		}
		catch (std::exception &e)
		{
			if (std::string("no fragment") != std::string(e.what()))
				throw ;
			iter = check_point;
		}
	}

	if (iter != end)
		throw (std::invalid_argument("invalid uri"));

}

URI::~URI()
{
}

void	URI::identifyURIType(void)
{
	//	identify relative path reference.
	if (this->_tokens[0] != "/")
	{
		if (this->_tokens.size() == 1)
		{
			this->_type = RELATIVE_PATH;
			return ;
		}
		if (this->_tokens[1] != ":")
		{
			this->_type = RELATIVE_PATH;
			return ;
		}
	}
	//	identify absolute path reference.
	if (this->_tokens[0] == "/" && ((this->_tokens.size() > 1 && this->_tokens[1] != "/") || this->_tokens.size() == 1))
	{
		this->_type = ABSOLUTE_PATH;
		return ;
	}
	//	identify network path reference.
	if (this->_tokens[0] == "/" && this->_tokens.size() > 1 && this->_tokens[1] == "/")
	{
		this->_type = NETWORK_PATH;
		return ;
	}
	this->_type = FULL_FORM;
}


Scheme		&URI::getScheme(void)
{
	if (!this->_isSchemeSet)
		throw (std::out_of_range("unset scheme"));
	return (this->_scheme);
}

Authority	&URI::getAuthority(void)
{
	if (!this->_isAuthoritySet)
		throw (std::out_of_range("unset authority"));
	return (this->_authority);
}

Path		&URI::getPath(void)
{
	return (this->_path);
}

Query		&URI::getQuery(void)
{
	return (this->_query);
}

Fragment	&URI::getFragment(void)
{
	return (this->_fragment);
}
