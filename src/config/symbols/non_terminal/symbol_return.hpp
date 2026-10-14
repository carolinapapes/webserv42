/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_return.hpp                                 :+:      :+:    :+:    */
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

class SymbolReturn : public ANonTerminal
{
	private:
		int			_statusCode;
		std::string	_bodyText;
	protected:
	public:
		SymbolReturn(void);
		~SymbolReturn(void);

		const char		*what(void) const;
		SymbolReturn	*clone(void) const;
		void			evaluate(Tree<AEvaluable*> *self);

		AParser	*getParser(void) const;

		int			getStatusCode(void) const ;
		std::string	getBodyText(void) const ;
};
