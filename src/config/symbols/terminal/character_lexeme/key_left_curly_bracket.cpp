/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_left_curly_bracket.cpp                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <key_left_curly_bracket.hpp>

KeyLeftCurlyBracket::KeyLeftCurlyBracket(void) : AKeyWord("{")
{
}

KeyLeftCurlyBracket::~KeyLeftCurlyBracket(void)
{
}

ATerminal* KeyLeftCurlyBracket::clone() const
{
    return new KeyLeftCurlyBracket(*this);
}

const char	*KeyLeftCurlyBracket::what(void) const
{
	return ("left_curly_bracket");
}
