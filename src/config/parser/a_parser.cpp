/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   a_parser.cpp                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <a_parser.hpp>

AParser::AParser(ANonTerminal *goal, AParsingRule *rule)
{
	this->_goal = goal;
	this->_rule = rule;
}

AParser::~AParser(void)
{
	delete (this->_goal);
	delete (this->_rule);
}

Tree<AEvaluable*>	*AParser::generateSubTree(terminal_iter &iter, const terminal_iter &end)
{
	std::vector<Tree<AEvaluable*>*>	subtrees = this->_rule->consumeTerminals(iter, end);

	Tree<AEvaluable*>	*rootNode = Tree<AEvaluable*>::makeTreeNode(this->_goal->clone());

	size_t	i = 0;
	while (i < subtrees.size())
	{
		rootNode->addTreeAsChild(subtrees[i]);
		i++;
	}
	return (rootNode);
}
