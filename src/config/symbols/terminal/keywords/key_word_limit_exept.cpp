/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_limit_exept.cpp                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <key_word_limit_exept.hpp>

KeyWordLimitExept::KeyWordLimitExept(void) : AKeyWord("limit_exept")
{
}

KeyWordLimitExept::~KeyWordLimitExept(void)
{
}

ATerminal*	KeyWordLimitExept::clone(void) const
{
	return (new KeyWordLimitExept(*this));
}

const char	*KeyWordLimitExept::what(void) const
{
	return ("limit_exept");
}
