/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_location.cpp                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <key_word_location.hpp>

KeyWordLocation::KeyWordLocation(void) : AKeyWord("location")
{
}

KeyWordLocation::~KeyWordLocation(void)
{
}

ATerminal*	KeyWordLocation::clone(void) const
{
	return (new KeyWordLocation(*this));
}

const char	*KeyWordLocation::what(void) const
{
	return ("location");
}
