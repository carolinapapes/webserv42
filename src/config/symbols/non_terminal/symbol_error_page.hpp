/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_error_page.hpp                             :+:      :+:    :+:    */
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

class SymbolErrorPage : public ANonTerminal
{
	private:
		std::vector<t_error_page>	_error_pages;
	protected:
	public:
		SymbolErrorPage(void);
		~SymbolErrorPage(void);

		const char		*what(void) const;
		SymbolErrorPage	*clone(void) const;
		void			evaluate(Tree<AEvaluable*> *self);

		AParser	*getParser(void) const;
		std::vector<t_error_page>	getErrorPages(void) const ;
};
