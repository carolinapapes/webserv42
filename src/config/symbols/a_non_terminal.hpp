/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   a_non_terminal.hpp                                :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <a_symbol.hpp>
#include <a_terminal.hpp>
#include <a_evaluable.hpp>
#include <tree.hpp>
#include <parse_exception.hpp>

//	be able to create them with a iterator of the terminalList.

class AParser;

class ANonTerminal : public AEvaluable
{
	private:
	protected:
	public:
		ANonTerminal(void);
		virtual	~ANonTerminal(void) = 0;

		virtual ANonTerminal*	clone(void) const = 0;

		virtual	AParser	*getParser(void) const = 0;
};
