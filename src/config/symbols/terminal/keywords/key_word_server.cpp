/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_server.cpp                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "key_word_server.hpp"
#include <cctype>

KeyWordServer::KeyWordServer(void) : AKeyWord("server")
{
}

KeyWordServer::~KeyWordServer(void)
{
}

ATerminal* KeyWordServer::clone() const
{
	return new KeyWordServer(*this);
}

const char	*KeyWordServer::what(void) const
{
	return ("server");
}
