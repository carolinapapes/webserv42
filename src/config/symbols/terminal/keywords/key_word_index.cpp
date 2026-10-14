/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_index.cpp                                :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <key_word_index.hpp>

KeyWordIndex::KeyWordIndex(void) : AKeyWord("index")
{
}

KeyWordIndex::~KeyWordIndex(void)
{
}

ATerminal*	KeyWordIndex::clone(void) const
{
	return (new KeyWordIndex(*this));
}

const char	*KeyWordIndex::what(void) const
{
	return ("index");
}
