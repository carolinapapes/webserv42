/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_client_max_body_size.hpp                   :+:      :+:    :+:    */
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

class SymbolClientMaxBodySize : public ANonTerminal
{
	private:
		size_t	_max_body_size;
	protected:
	public:
		SymbolClientMaxBodySize(void);
		~SymbolClientMaxBodySize(void);

		const char		*what(void) const;
		SymbolClientMaxBodySize	*clone(void) const;
		void			evaluate(Tree<AEvaluable*> *self);

		AParser	*getParser(void) const;

		size_t	getMaxBodySize(void) const ;
};
