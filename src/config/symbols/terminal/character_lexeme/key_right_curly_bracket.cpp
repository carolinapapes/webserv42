/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_right_curly_bracket.cpp                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <key_right_curly_bracket.hpp>

KeyRightCurlyBracket::KeyRightCurlyBracket(void) : AKeyWord("}")
{
}

KeyRightCurlyBracket::~KeyRightCurlyBracket(void)
{
}

ATerminal*	KeyRightCurlyBracket::clone(void) const
{
	return new KeyRightCurlyBracket(*this);
}

const char	*KeyRightCurlyBracket::what(void) const
{
	return ("right_curly_bracket");
}
