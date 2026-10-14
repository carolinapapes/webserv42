/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_root.hpp                                   :+:      :+:    :+:    */
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

class SymbolRoot : public ANonTerminal
{
	private:
		std::string	_path;
	protected:
	public:
		SymbolRoot(void);
		~SymbolRoot(void);

		const char		*what(void) const;
		SymbolRoot	*clone(void) const;
		void			evaluate(Tree<AEvaluable*> *self);

		AParser	*getParser(void) const;

		std::string	getPath(void) const ;
};
