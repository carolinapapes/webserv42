/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parsing_rule_or.hpp                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <cstdarg>
#include <a_parsing_rule.hpp>
#include <a_symbol.hpp>

class ParsingRuleOr : public AParsingRule
{
	private:
		std::vector<AParsingRule *>	_rules;
	protected:
	public:
		ParsingRuleOr(AParsingRule *rule, ...); // needs to be null terminated.
		~ParsingRuleOr(void);

		std::vector<Tree<AEvaluable*>*>	consumeTerminals(terminal_iter &iter, const terminal_iter &end) const;
};
