/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   a_terminal.cpp                                    :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */


#include <a_terminal.hpp>

ATerminal::ATerminal(void) : ASymbol()
{
	_text = "";
}


static std::string	remove_quotes(std::string text)
{
	std::string	new_text = "";

	size_t	i = 0;
	while (i < text.size())
	{
		if (text[i] == '"')
			i++;
		else
		{
			new_text += text.substr(i, text.substr(i).find('"'));
			if (text.find('"') == std::string::npos)
				i = text.size();
			else
				i += text.substr(i).find('"');
		}
	}
	return (new_text);
}

void	ATerminal::setText(const std::string & text)
{
	this->_text = remove_quotes(text);
}

std::string	ATerminal::getText(void) const
{
	return (_text);
}

size_t		ATerminal::getTextLength(void) const
{
	return (_text.size());
}
