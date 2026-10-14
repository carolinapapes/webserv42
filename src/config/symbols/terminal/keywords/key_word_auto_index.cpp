/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_auto_index.cpp                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <key_word_auto_index.hpp>

KeyWordAutoIndex::KeyWordAutoIndex(void) : AKeyWord("auto_index")
{
}

KeyWordAutoIndex::~KeyWordAutoIndex(void)
{
}

ATerminal*	KeyWordAutoIndex::clone(void) const
{
	return (new KeyWordAutoIndex(*this));
}

const char	*KeyWordAutoIndex::what(void) const
{
	return ("auto_index");
}
