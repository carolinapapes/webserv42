/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_error_page.cpp                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>
#include <symbol_error_page.hpp>

#include <key_word_error_page.hpp>
#include <number.hpp>
#include <text_config_file.hpp>
#include <key_semicolon.hpp>

#include <a_parser.hpp>
#include <a_parsing_rule.hpp>
#include <parsing_rule_symbol.hpp>
#include <parsing_rule_and.hpp>
#include <parsing_rule_or.hpp>
#include <parsing_rule_repetition.hpp>

#include <utils.hpp>
#include <path.hpp>

SymbolErrorPage::SymbolErrorPage(void)
{
}

SymbolErrorPage::~SymbolErrorPage(void)
{
}

const char	*SymbolErrorPage::what(void) const
{
	return ("error_page_symbol");
}

SymbolErrorPage	*SymbolErrorPage::clone(void) const
{
	return (new SymbolErrorPage(*this));
}

void		SymbolErrorPage::evaluate(Tree<AEvaluable*> *self)
{
	TextConfigFile	*text;
	if (self->getChildNodeSize() < 2)
		throw (std::invalid_argument("incorrect arrguments to error page."));
	text = dynamic_cast<TextConfigFile *>(self->getChildNode(self->getChildNodeSize() - 1)->getContent());
	if (!text)
		throw (std::invalid_argument("incorrect arrguments to error page."));
	Path	file_path(text->getText());
	size_t	i = 0;
	while (i < (self->getChildNodeSize() - 1))
	{
		Number	*number_token;
		number_token = dynamic_cast<Number *>(self->getChildNode(i)->getContent());
		if (!number_token)
			throw (std::invalid_argument("incorrect arrguments to error page."));
		t_status_code status_code = stricter_unsigned_stoi(number_token->getText(), (size_t *)std::string::npos);
		t_error_page error_page;
		error_page.uri = file_path.getPathText();
		error_page.status_code = status_code;
		this->_error_pages.push_back(error_page);
		i++;
	}
}

AParser	*SymbolErrorPage::getParser(void) const
{
	AParsingRule	*rule =	new ParsingRuleAnd(\
			new ParsingRuleSymbol(KeyWordErrorPage().clone()),\
			new ParsingRuleRepetition(1, -1, new ParsingRuleSymbol(Number().clone())),\
			new ParsingRuleSymbol(TextConfigFile().clone()),\
			new ParsingRuleSymbol(KeySemicolon().clone()),\
			NULL);
	return (new AParser(this->clone(), rule));
}

std::vector<t_error_page>	SymbolErrorPage::getErrorPages(void) const
{
	return (this->_error_pages);
}
