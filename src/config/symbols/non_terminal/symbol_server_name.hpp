/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_server_name.hpp                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>

class SymbolServerName : public ANonTerminal
{
	private:
		std::vector<std::string>	_names;
	protected:
	public:
		SymbolServerName(void);
		~SymbolServerName(void);

		const char		*what(void) const;
		SymbolServerName	*clone(void) const;
		void			evaluate(Tree<AEvaluable*> *self);

		AParser	*getParser(void) const;
		std::vector<std::string>	getNames(void) const ;
};
