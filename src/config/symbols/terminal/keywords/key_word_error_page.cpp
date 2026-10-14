/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_error_page.cpp                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "key_word_error_page.hpp"
#include <cctype>

KeyWordErrorPage::KeyWordErrorPage(void) : AKeyWord("error_page")
{
}

KeyWordErrorPage::~KeyWordErrorPage(void)
{
}

ATerminal* KeyWordErrorPage::clone() const
{
	return new KeyWordErrorPage(*this);
}

const char	*KeyWordErrorPage::what(void) const
{
	return ("error_page");
}
