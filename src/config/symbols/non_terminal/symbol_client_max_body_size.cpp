/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_client_max_body_size.cpp                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>
#include <symbol_client_max_body_size.hpp>

#include <key_word_client_max_body_size.hpp>
#include <text_config_file.hpp>
#include <key_semicolon.hpp>

#include <a_parser.hpp>
#include <a_parsing_rule.hpp>
#include <parsing_rule_symbol.hpp>
#include <parsing_rule_and.hpp>
#include <parsing_rule_or.hpp>
#include <parsing_rule_repetition.hpp>

#include <utils.hpp>

SymbolClientMaxBodySize::SymbolClientMaxBodySize(void)
{
}

SymbolClientMaxBodySize::~SymbolClientMaxBodySize(void)
{
}

const char	*SymbolClientMaxBodySize::what(void) const
{
	return ("client_max_body_size_symbol");
}

SymbolClientMaxBodySize	*SymbolClientMaxBodySize::clone(void) const
{
	return (new SymbolClientMaxBodySize(*this));
}

static size_t	get_file_size_unit(std::string unit_str)
{
	if (unit_str == "")
		return (1);
	if (unit_str == "KiB")
		return (1024);
	if (unit_str == "MiB")
		return (1048576);
	if (unit_str == "GiB")
		return (1073741824);
	return (0);
}

static size_t	parse_file_size(std::string file_size_str)
{
	size_t	i = 0;
	size_t	body_size = stoi(file_size_str, &i);
	body_size = body_size * get_file_size_unit(file_size_str.substr(i));
	return (body_size);
}

void		SymbolClientMaxBodySize::evaluate(Tree<AEvaluable*> *self)
{
	if (self->getChildNodeSize() != 1)
		throw (std::invalid_argument("incorrect arrguments to client max body size."));
	TextConfigFile	*text = dynamic_cast<TextConfigFile *>(self->getChildNode(0)->getContent());
	if (!text)
		throw (std::invalid_argument("incorrect arrguments to client max body size."));
	this->_max_body_size = parse_file_size(text->getText());
	if (this->_max_body_size == 0)
		throw (std::invalid_argument("incorect file size unit."));
}

AParser	*SymbolClientMaxBodySize::getParser(void) const
{
	AParsingRule	*rule =	new ParsingRuleAnd(\
			new ParsingRuleSymbol(KeyWordClientMaxBodySize().clone()),\
			new ParsingRuleSymbol(TextConfigFile().clone()),\
			new ParsingRuleSymbol(KeySemicolon().clone()),\
			NULL);
	return (new AParser(this->clone(), rule));
}

size_t	SymbolClientMaxBodySize::getMaxBodySize(void) const
{
	return (this->_max_body_size);
}
