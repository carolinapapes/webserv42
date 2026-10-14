/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_return.cpp                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "key_word_return.hpp"
#include <cctype>

KeyWordReturn::KeyWordReturn(void) :AKeyWord("return")
{
}

KeyWordReturn::~KeyWordReturn(void)
{
}

ATerminal* KeyWordReturn::clone() const
{
	return new KeyWordReturn(*this);
}

const char	*KeyWordReturn::what(void) const
{
	return ("return");
}
