/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   a_terminal_factory.hpp                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <a_terminal.hpp>
#include <string>
#include <vector>

class ATerminalFactory
{
	private:
		size_t	_line;
		size_t	_column;
		std::vector<ATerminal *>	terminals;
	protected:
		void	addTerminalToFactory(ATerminal * terminal);

		void	updateIndex(const std::string & str);
		ATerminalFactory(void);
	public:
		~ATerminalFactory(void);

		ATerminal*	createTerminal(const std::string & str);

		void	resetIndex(void);
};
