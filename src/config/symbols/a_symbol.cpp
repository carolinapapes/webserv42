/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   a_symbol.cpp                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include <a_symbol.hpp>

ASymbol::ASymbol(void)
{
	this->_size = 0;
	this->_line = 0;
	this->_column = 0;
}

ASymbol::~ASymbol(void)
{
}

void	ASymbol::setSize(size_t n)
{
	this->_size = n;
}

void	ASymbol::setLine(size_t n)
{
	this->_line = n;
}

void	ASymbol::setColumn(size_t n)
{
	this->_column = n;
}

size_t	ASymbol::getSize(void) const
{
	return (this->_size);
}

size_t	ASymbol::getLine(void) const
{
	return (this->_line);
}

size_t	ASymbol::getColumn(void) const
{
	return (this->_column);
}
