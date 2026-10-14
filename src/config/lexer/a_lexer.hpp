/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   a_lexer.hpp                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>

#include <a_symbol.hpp>
#include <a_terminal.hpp>
#include <a_terminal_factory.hpp>

class ALexer
{
	private:
	protected:
		ATerminalFactory*	_factory;
		ALexer(void);
	public:
		~ALexer(void);

		std::vector<ATerminal*>	tokenizeStr(const std::string & str) const ;
};
