/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parsing_rule_symbol.cpp                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <parsing_rule_symbol.hpp>
#include <parse_exception.hpp>
#include <tree.hpp>
#include <a_non_terminal.hpp>
#include <a_parser.hpp>

#include <symbol_return.hpp>

ParsingRuleSymbol::ParsingRuleSymbol(const ASymbol *symbol)
{
	this->_symbol = symbol->clone();
	delete (symbol);
}

ParsingRuleSymbol::~ParsingRuleSymbol(void)
{
	delete (this->_symbol);
}

std::vector<Tree<AEvaluable*>*>	ParsingRuleSymbol::consumeTerminals(terminal_iter &iter, const terminal_iter &end) const
{
	std::vector<Tree<AEvaluable*>*>	return_val;
	if (iter == end)
	{
		iter--;
		parse_exception e(ALL_SYMBOLS_CONSUMED);
		e.setLine((*iter)->getLine());
		e.setColumn((*iter)->getColumn() + (*iter)->getSize());
		e.setSize(1);

		throw (e);
	}
	//	check if we need to generate any subtrees for this symbol.
	ANonTerminal	*nonTerminal = dynamic_cast<ANonTerminal*>(this->_symbol);
	if (nonTerminal)
	{
		AParser	*parser = nonTerminal->getParser();
		try
		{
			return_val.push_back(parser->generateSubTree(iter, end));
		}
		catch (...)
		{
			delete (parser);
			throw ;
		}
		delete (parser);
		return (return_val);
	}
	//	check if this simbol is in the current iterator of the terminal list.
	ASymbol	*symbol = dynamic_cast<ASymbol*>(*iter);
	if (((void **)this->_symbol)[0] != ((void **)symbol)[0])
	{
		parse_exception e(UNRECOGNIZED_SYMBOL);
		e.setLine((*iter)->getLine());
		e.setColumn((*iter)->getColumn());
		e.setSize((*iter)->getSize());

		throw (e);
	}
	//	check if we need to store it in the ast or not.
	AEvaluable	*evaluable = dynamic_cast<AEvaluable*>(this->_symbol);
	if (evaluable)
	{
		return_val.push_back(Tree<AEvaluable*>::makeTreeNode((dynamic_cast<AEvaluable*>(*iter))->clone()));
	}
	iter++;
	return (return_val);
}
