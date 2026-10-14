/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_index.hpp                                  :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>

class SymbolIndex : public ANonTerminal
{
	private:
		std::vector<std::string>	_indexFiles;
	protected:
	public:
		SymbolIndex(void);
		~SymbolIndex(void);

		const char		*what(void) const;
		SymbolIndex	*clone(void) const;
		void			evaluate(Tree<AEvaluable*> *self);

		AParser	*getParser(void) const;

		std::vector<std::string>	getIndexFiles(void) const ;
};
