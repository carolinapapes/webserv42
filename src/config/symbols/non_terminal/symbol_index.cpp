/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_index.cpp                                  :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>
#include <symbol_index.hpp>

#include <key_word_index.hpp>
#include <text_config_file.hpp>
#include <key_semicolon.hpp>

#include <symbol_index.hpp>

#include <a_parser.hpp>
#include <a_parsing_rule.hpp>
#include <parsing_rule_symbol.hpp>
#include <parsing_rule_and.hpp>
#include <parsing_rule_or.hpp>
#include <parsing_rule_repetition.hpp>

#include <string>
#include <vector>
#include <stdexcept>
#include <path.hpp>

SymbolIndex::SymbolIndex(void)
{
}

SymbolIndex::~SymbolIndex(void)
{
}

const char	*SymbolIndex::what(void) const
{
	return ("index_symbol");
}

SymbolIndex	*SymbolIndex::clone(void) const
{
	return (new SymbolIndex(*this));
}

void		SymbolIndex::evaluate(Tree<AEvaluable*> *self)
{
	TextConfigFile	*text;

	//	error checking.
	size_t	i = 0;
	while (i < self->getChildNodeSize())
	{
		text = dynamic_cast<TextConfigFile *>(self->getChildNode(i)->getContent());
		if (!text)
			throw (std::invalid_argument("incorrect arrguments to index."));
		Path file(text->getText());
		if (file.getSections().size() != 1)
			throw (std::invalid_argument("incorrect arrguments to index."));
		this->_indexFiles.push_back(file.getPathText());
		i++;
	}
}

AParser	*SymbolIndex::getParser(void) const
{
	AParsingRule	*rule =	new ParsingRuleAnd(\
			new ParsingRuleSymbol(KeyWordIndex().clone()),\
			new ParsingRuleRepetition(0, -1, new ParsingRuleSymbol(TextConfigFile().clone())),\
			new ParsingRuleSymbol(KeySemicolon().clone()),\
			NULL);
	return (new AParser(this->clone(), rule));
}

std::vector<std::string>	SymbolIndex::getIndexFiles(void) const
{
	return (this->_indexFiles);
}
