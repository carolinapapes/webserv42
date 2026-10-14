/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_server_name.cpp                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "key_word_server_name.hpp"
#include <cctype>

KeyWordServerName::KeyWordServerName(void) : AKeyWord("server_name")
{
}

KeyWordServerName::~KeyWordServerName(void)
{
}

ATerminal* KeyWordServerName::clone() const
{
	return new KeyWordServerName(*this);
}

const char	*KeyWordServerName::what(void) const
{
	return ("server_name");
}
