/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   number.cpp                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include <string>
#include <cctype>
#include <cstdlib>

#include <number.hpp>

Number::Number(void) : ATerminal()
{
}

Number::~Number(void)
{
}

Number* Number::clone() const
{
	return new Number(*this);
}

size_t	Number::getTerminalSizeOnStr(const std::string & str) const
{
	size_t	i = 0;
	char	*c_str = (char *) str.c_str();

	while (std::isdigit(c_str[i]))
	{
		i++;
	}
	return (i);
}

const char	*Number::what(void) const
{
	return ("number");
}

void	Number::evaluate(Tree<AEvaluable*> *self)
{
	(void) self;
}
