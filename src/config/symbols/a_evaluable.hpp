/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   a_evaluable.hpp                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <a_symbol.hpp>
#include <tree.hpp>

class AEvaluable : virtual public ASymbol
{
	private:
	protected:
	public:
		AEvaluable(void);
		virtual	~AEvaluable(void);

		virtual AEvaluable*	clone(void) const = 0;

		virtual void	evaluate(Tree<AEvaluable*> *self) = 0;
};
