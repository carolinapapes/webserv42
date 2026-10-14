/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_auto_index.cpp                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>
#include <symbol_auto_index.hpp>

#include <key_word_auto_index.hpp>
#include <number.hpp>
#include <text_config_file.hpp>
#include <key_semicolon.hpp>

#include <symbol_auto_index.hpp>

#include <a_parser.hpp>
#include <a_parsing_rule.hpp>
#include <parsing_rule_symbol.hpp>
#include <parsing_rule_and.hpp>
#include <parsing_rule_or.hpp>
#include <parsing_rule_repetition.hpp>

#include <stdexcept>

SymbolAutoIndex::SymbolAutoIndex(void)
{
}

SymbolAutoIndex::~SymbolAutoIndex(void)
{
}

const char	*SymbolAutoIndex::what(void) const
{
	return ("auto_index_symbol");
}

SymbolAutoIndex	*SymbolAutoIndex::clone(void) const
{
	return (new SymbolAutoIndex(*this));
}

void		SymbolAutoIndex::evaluate(Tree<AEvaluable*> *self)
{
	TextConfigFile	*value;
	this->_isSet = 0;

	//	error checking.
	if (self->getChildNodeSize() != 1)
		throw (std::invalid_argument("incorrect arrguments to autoindex."));
	if (!dynamic_cast<TextConfigFile *>(self->getChildNode(0)->getContent()))
		throw (std::invalid_argument("incorrect arrguments to autoindex."));

	value = dynamic_cast<TextConfigFile *>(self->getChildNode(0)->getContent());
	if (value->getText() == "on")
		this->_isSet = 1;
	else if (value->getText() == "off")
		this->_isSet = 0;
	else
		throw (std::invalid_argument("incorrect arrguments to autoindex."));
}

AParser	*SymbolAutoIndex::getParser(void) const
{
	AParsingRule	*rule =	new ParsingRuleAnd(\
			new ParsingRuleSymbol(KeyWordAutoIndex().clone()),\
			new ParsingRuleSymbol(TextConfigFile().clone()),\
			new ParsingRuleSymbol(KeySemicolon().clone()),\
			NULL);
	return (new AParser(this->clone(), rule));
}

int		SymbolAutoIndex::getIsSet(void) const
{
	return (this->_isSet);
}
