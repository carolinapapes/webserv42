/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_auto_index.hpp                             :+:      :+:    :+:    */
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

class SymbolAutoIndex : public ANonTerminal
{
	private:
		int	_isSet;
	protected:
	public:
		SymbolAutoIndex(void);
		~SymbolAutoIndex(void);

		const char		*what(void) const;
		SymbolAutoIndex	*clone(void) const;
		void			evaluate(Tree<AEvaluable*> *self);

		AParser	*getParser(void) const;

		int	getIsSet(void) const ;
};
