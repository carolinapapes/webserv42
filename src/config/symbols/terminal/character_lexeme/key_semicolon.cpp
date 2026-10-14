/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_semicolon.cpp                                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <key_semicolon.hpp>

KeySemicolon::KeySemicolon(void) : AKeyWord(";")
{
}

KeySemicolon::~KeySemicolon(void)
{
}

ATerminal* KeySemicolon::clone(void) const
{
	return (new KeySemicolon(*this));
}

const char	*KeySemicolon::what(void) const
{
	return ("semicolon");
}
