/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   a_terminal.hpp                                    :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <a_symbol.hpp>


class ATerminal : virtual public ASymbol
{
	private:
	protected:
		std::string _text;
	public:
		ATerminal(void);

		//	this will find the size that the terminal should have on the string.
		virtual size_t		getTerminalSizeOnStr(const std::string & str) const = 0;

		//	this only worw over the _text variable.
		void		setText(const std::string & text);
		std::string	getText(void) const;
		size_t		getTextLength(void) const;

};

typedef std::vector<ATerminal*>::iterator terminal_iter;
