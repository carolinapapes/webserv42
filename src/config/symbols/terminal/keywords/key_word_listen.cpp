/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_listen.cpp                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "key_word_listen.hpp"
#include <cctype>

KeyWordListen::KeyWordListen(void) : AKeyWord("listen")
{
}

KeyWordListen::~KeyWordListen(void)
{
}

ATerminal* KeyWordListen::clone() const
{
	return new KeyWordListen(*this);
}

const char	*KeyWordListen::what(void) const
{
	return ("listen");
}
