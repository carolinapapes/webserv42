/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_server_name.cpp                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>
#include <symbol_server_name.hpp>

#include <key_word_server_name.hpp>
#include <text_config_file.hpp>
#include <key_semicolon.hpp>

#include <a_parser.hpp>
#include <a_parsing_rule.hpp>
#include <parsing_rule_symbol.hpp>
#include <parsing_rule_and.hpp>
#include <parsing_rule_or.hpp>
#include <parsing_rule_repetition.hpp>

SymbolServerName::SymbolServerName(void)
{
}

SymbolServerName::~SymbolServerName(void)
{
}

const char	*SymbolServerName::what(void) const
{
	return ("server_name_symbol");
}

SymbolServerName	*SymbolServerName::clone(void) const
{
	return (new SymbolServerName(*this));
}

void		SymbolServerName::evaluate(Tree<AEvaluable*> *self)
{
	TextConfigFile	*text;

	size_t	i = 0;
	while (i < self->getChildNodeSize())
	{
		text = dynamic_cast<TextConfigFile *>(self->getChildNode(i)->getContent());
		if (!text)
			throw (std::invalid_argument("incorrect arrguments to limitexcept."));
		this->_names.push_back(text->getText());
		i++;
	}
}

AParser	*SymbolServerName::getParser(void) const
{
	AParsingRule	*rule =	new ParsingRuleAnd(\
			new ParsingRuleSymbol(KeyWordServerName().clone()),\
			new ParsingRuleRepetition(1, -1, new ParsingRuleSymbol(TextConfigFile().clone())),\
			new ParsingRuleSymbol(KeySemicolon().clone()),\
			NULL);
	return (new AParser(this->clone(), rule));
}

std::vector<std::string>	SymbolServerName::getNames(void) const
{
	return (this->_names);
}
