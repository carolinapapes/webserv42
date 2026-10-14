/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_client_max_body_size.cpp                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <key_word_client_max_body_size.hpp>

KeyWordClientMaxBodySize::KeyWordClientMaxBodySize(void) : AKeyWord("client_max_body_size")
{
}

KeyWordClientMaxBodySize::~KeyWordClientMaxBodySize(void)
{
}

ATerminal*	KeyWordClientMaxBodySize::clone(void) const
{
	return (new KeyWordClientMaxBodySize(*this));
}

const char	*KeyWordClientMaxBodySize::what(void) const
{
	return ("client_cax_body_size");
}
