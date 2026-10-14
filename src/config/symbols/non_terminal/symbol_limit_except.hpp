/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_limit_except.hpp                           :+:      :+:    :+:    */
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

#include <webserv/intermediate_representation.hpp>

class SymbolLimitExcept : public ANonTerminal
{
	private:
		std::vector<t_http_method>	_methods;
	protected:
	public:
		SymbolLimitExcept(void);
		~SymbolLimitExcept(void);

		const char			*what(void) const;
		SymbolLimitExcept	*clone(void) const;
		void				evaluate(Tree<AEvaluable*> *self);

		AParser	*getParser(void) const;

		std::vector<t_http_method>	getMethods(void) const ;
};
