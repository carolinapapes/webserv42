/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_config_context.cpp                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>
#include <symbol_config_context.hpp>

#include <symbol_server.hpp>


#include <a_parser.hpp>
#include <a_parsing_rule.hpp>
#include <parsing_rule_symbol.hpp>
#include <parsing_rule_and.hpp>
#include <parsing_rule_or.hpp>
#include <parsing_rule_repetition.hpp>

#include <webserv/defines.hpp>

SymbolConfigContext::SymbolConfigContext(void)
{
}

SymbolConfigContext::~SymbolConfigContext(void)
{
}

const char	*SymbolConfigContext::what(void) const
{
	return ("configuration context");
}

SymbolConfigContext	*SymbolConfigContext::clone(void) const
{
	return (new SymbolConfigContext(*this));
}

#include <ir_printing_functions.hpp>

void		SymbolConfigContext::evaluate(Tree<AEvaluable*> *self)
{
	size_t	i = 0;
	while (i < self->getChildNodeSize())
	{
		this->_servers.push_back(dynamic_cast<SymbolServer *>(self->getChildNode(i)->getContent())->getServer());
		i++;
	}

	if (!DEBUG_PRINT)
		return ;
	i = 0;
	while (i < this->_servers.size())
	{
		printServer(this->_servers[i]);
		i++;
	}
}

AParser	*SymbolConfigContext::getParser(void) const
{
	AParsingRule	*rule = new ParsingRuleRepetition(1, -1, \
			new ParsingRuleSymbol(SymbolServer().clone()));
	return (new AParser(this->clone(), rule));
}

std::vector<t_server>	SymbolConfigContext::getServers(void) const
{
	return (this->_servers);
}
