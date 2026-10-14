/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parsing_rule_and.cpp                              :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <parsing_rule_and.hpp>
#include <parse_exception.hpp>
#include <tree.hpp>
#include <a_non_terminal.hpp>
#include <a_parser.hpp>


ParsingRuleAnd::ParsingRuleAnd(AParsingRule *first_rule, ...)
{
	va_list args;
	va_start(args, first_rule);

	this->_rules.push_back(first_rule);
	AParsingRule	*rule = va_arg(args, AParsingRule *);
	while (rule)
	{
		this->_rules.push_back(rule);
		rule = va_arg(args, AParsingRule *);
	}
}

ParsingRuleAnd::~ParsingRuleAnd(void)
{
	size_t	i = 0;
	while (i < this->_rules.size())
	{
		delete (this->_rules[i]);
		i++;
	}
}

std::vector<Tree<AEvaluable*>*>	ParsingRuleAnd::consumeTerminals(terminal_iter &iter, const terminal_iter &end) const
{
	std::vector<Tree<AEvaluable*>*>	return_val;
	std::vector<Tree<AEvaluable*>*>	temp_vector;
	size_t	i = 0;
	try
	{
		while (i < this->_rules.size())
		{
			temp_vector = this->_rules[i]->consumeTerminals(iter, end);
			return_val.insert(return_val.end(), temp_vector.begin(), temp_vector.end());
			temp_vector = std::vector<Tree<AEvaluable*>*>();
			i++;
		}
	}
	catch (...)
	{
		i = 0;
		while (i < return_val.size())
		{
			delete (return_val[i]);
			i++;
		}
		i = 0;
		while (i < temp_vector.size())
		{
			delete (temp_vector[i]);
			i++;
		}
		throw ;
	}
	return (return_val);
}
