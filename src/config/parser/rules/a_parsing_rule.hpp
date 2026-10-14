/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   a_parsing_rule.hpp                                :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <tree.hpp>
#include <vector>
#include <a_evaluable.hpp>
#include <a_terminal.hpp>

class AParsingRule
{
	private:
	protected:
	public:

		AParsingRule(void);
		virtual ~AParsingRule(void);

		virtual std::vector<Tree<AEvaluable*>*>	consumeTerminals(terminal_iter &iter, const terminal_iter &end) const = 0;
};
