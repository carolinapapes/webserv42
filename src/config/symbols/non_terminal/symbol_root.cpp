/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_root.cpp                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>
#include <symbol_root.hpp>

#include <key_word_root.hpp>
#include <text_config_file.hpp>
#include <key_semicolon.hpp>

#include <symbol_root.hpp>

#include <a_parser.hpp>
#include <a_parsing_rule.hpp>
#include <parsing_rule_symbol.hpp>
#include <parsing_rule_and.hpp>
#include <parsing_rule_or.hpp>
#include <parsing_rule_repetition.hpp>

#include <stdexcept>
#include <path.hpp>

SymbolRoot::SymbolRoot(void)
{
}

SymbolRoot::~SymbolRoot(void)
{
}

const char	*SymbolRoot::what(void) const
{
	return ("root_symbol");
}

SymbolRoot	*SymbolRoot::clone(void) const
{
	return (new SymbolRoot(*this));
}

void		SymbolRoot::evaluate(Tree<AEvaluable*> *self)
{
	TextConfigFile	*root_path_text;

	//	error checking.
	if (self->getChildNodeSize() != 1)
		throw (std::invalid_argument("incorrect arrguments to root."));
	if (!dynamic_cast<TextConfigFile *>(self->getChildNode(0)->getContent()))
		throw (std::invalid_argument("incorrect arrguments to root."));

	root_path_text = dynamic_cast<TextConfigFile *>(self->getChildNode(0)->getContent());

	Path	root_path(root_path_text->getText());
	this->_path = root_path.getPathText();
}

AParser	*SymbolRoot::getParser(void) const
{
	AParsingRule	*rule =	new ParsingRuleAnd(\
			new ParsingRuleSymbol(KeyWordRoot().clone()),\
			new ParsingRuleSymbol(TextConfigFile().clone()),\
			new ParsingRuleSymbol(KeySemicolon().clone()),\
			NULL);
	return (new AParser(this->clone(), rule));
}

std::string	SymbolRoot::getPath(void) const
{
	return (this->_path);
}
