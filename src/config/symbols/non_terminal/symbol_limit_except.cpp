/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_limit_except.cpp                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>
#include <symbol_limit_except.hpp>

#include <key_word_limit_exept.hpp>
#include <text_config_file.hpp>
#include <key_semicolon.hpp>

#include <symbol_limit_except.hpp>

#include <a_parser.hpp>
#include <a_parsing_rule.hpp>
#include <parsing_rule_symbol.hpp>
#include <parsing_rule_and.hpp>
#include <parsing_rule_or.hpp>
#include <parsing_rule_repetition.hpp>

#include <webserv/intermediate_representation.hpp>
#include <stdexcept>

SymbolLimitExcept::SymbolLimitExcept(void)
{

}

SymbolLimitExcept::~SymbolLimitExcept(void)
{
}

const char	*SymbolLimitExcept::what(void) const
{
	return ("limit_except_symbol");
}

SymbolLimitExcept	*SymbolLimitExcept::clone(void) const
{
	return (new SymbolLimitExcept(*this));
}

static t_http_method	str_to_http_method(std::string text)
{
	if (text == "GET" || text == "get")
		return (GET);
	if (text == "POST" || text == "post")
		return (POST);
	if (text == "DELETE" || text == "delete")
		return (DELETE);
	return ((t_http_method)-1);
}

void		SymbolLimitExcept::evaluate(Tree<AEvaluable*> *self)
{
	TextConfigFile	*text;

	//	error checking.
	size_t	i = 0;
	while (i < self->getChildNodeSize())
	{
		text = dynamic_cast<TextConfigFile *>(self->getChildNode(i)->getContent());
		if (!text)
			throw (std::invalid_argument("incorrect arrguments to limitexcept."));
		t_http_method method = str_to_http_method(text->getText());
		if (method == -1)
			throw (std::invalid_argument("incorrect arrguments to limitexcept."));
		this->_methods.push_back(method);
		i++;
	}
}

AParser	*SymbolLimitExcept::getParser(void) const
{
	AParsingRule	*rule =	new ParsingRuleAnd(\
			new ParsingRuleSymbol(KeyWordLimitExept().clone()),\
			new ParsingRuleRepetition(1, -1, new ParsingRuleSymbol(TextConfigFile().clone())),\
			new ParsingRuleSymbol(KeySemicolon().clone()),\
			NULL);
	return (new AParser(this->clone(), rule));
}

std::vector<t_http_method>	SymbolLimitExcept::getMethods(void) const
{
	return (this->_methods);
}
