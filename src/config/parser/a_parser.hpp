/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   a_parser.hpp                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <tree.hpp>
#include <a_non_terminal.hpp>

#include <a_parsing_rule.hpp>
#include <parsing_rule_symbol.hpp>


class AParser
{
	private:
		//	codify the information about and, or, repetition, symple symbol.
		ANonTerminal	*_goal;
		AParsingRule	*_rule;

	protected:
	public:
		AParser(ANonTerminal *goal, AParsingRule *rule);
		~AParser(void);

		Tree<AEvaluable*>	*generateSubTree(terminal_iter &iter, const terminal_iter &end);
};
