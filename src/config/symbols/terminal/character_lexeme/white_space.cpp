/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   white_space.cpp                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include <white_space.hpp>

WhiteSpace::WhiteSpace(void) : ATerminal()
{
}

WhiteSpace::~WhiteSpace(void)
{
}

ATerminal* WhiteSpace::clone() const
{
    return new WhiteSpace(*this);
}

size_t	 WhiteSpace::getTerminalSizeOnStr(const std::string& str) const
{
	size_t	i = 0;
	char	*c_str = (char *) str.c_str();

	while (c_str[i] == ' ' || c_str[i] == '\t' || c_str[i] == '\r' || c_str[i] == '\n')
	{
		if (c_str[i] == '\r')
		{
			if (c_str[i + 1] != '\n')
				break ;
			i += 2;
		}
		else
			i++;
	}
	return (i);
}

const char	*WhiteSpace::what(void) const
{
	return ("white_space");
}
