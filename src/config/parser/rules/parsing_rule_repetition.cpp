/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parsing_rule_repetition.cpp                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <parsing_rule_repetition.hpp>
#include <parse_exception.hpp>
#include <tree.hpp>
#include <a_non_terminal.hpp>
#include <a_parser.hpp>

ParsingRuleRepetition::ParsingRuleRepetition(size_t	min, size_t	max, AParsingRule *rule)
{
	this->_min = min;
	this->_max = max;
	this->_rule = rule;
}

ParsingRuleRepetition::~ParsingRuleRepetition(void)
{
	delete (this->_rule);
}

std::vector<Tree<AEvaluable*>*>	ParsingRuleRepetition::consumeTerminals(terminal_iter &iter, const terminal_iter &end) const
{
	std::vector<Tree<AEvaluable*>*>	return_val;
	std::vector<Tree<AEvaluable*>*>	tmp_vector;
	terminal_iter	check_point;
	size_t	i = 0;
	while (i < this->_max)
	{
		try
		{
			check_point = iter;
			tmp_vector = this->_rule->consumeTerminals(iter, end);
			i++;
		}
		catch (parse_exception & e)
		{
			iter = check_point;
			if (i < this->_min || i > this->_max)
				throw ;
			break ;
		}
		return_val.insert(return_val.end(), tmp_vector.begin(), tmp_vector.end());
	}
	return (return_val);
}
