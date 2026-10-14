/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   number.hpp                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>

#include <a_terminal.hpp>
#include <a_evaluable.hpp>

class Number : public ATerminal, public AEvaluable
{
	private:
	protected:
	public:
		Number(void);
		~Number(void);

		const char	*what(void) const ;
		size_t		getTerminalSizeOnStr(const std::string & str) const;
		Number*	clone(void) const;

		void	evaluate(Tree<AEvaluable*> *self);
};
