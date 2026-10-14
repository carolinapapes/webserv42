/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_root.cpp                                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <key_word_root.hpp>

KeyWordRoot::KeyWordRoot(void) : AKeyWord("root")
{
}

KeyWordRoot::~KeyWordRoot(void)
{
}

ATerminal*	KeyWordRoot::clone(void) const
{
	return (new KeyWordRoot(*this));
}

const char	*KeyWordRoot::what(void) const
{
	return ("root");
}
