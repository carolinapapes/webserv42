/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parsing_rule_repetition.hpp                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <a_parsing_rule.hpp>
#include <a_symbol.hpp>

class ParsingRuleRepetition : public AParsingRule
{
	private:
		size_t			_min;
		size_t			_max;
		AParsingRule	*_rule;
	protected:
	public:
		ParsingRuleRepetition(size_t min, size_t max, AParsingRule *rule);
		~ParsingRuleRepetition(void);

		std::vector<Tree<AEvaluable*>*>	consumeTerminals(terminal_iter &iter, const terminal_iter &end) const;
};
