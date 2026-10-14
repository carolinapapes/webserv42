/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   text_config_file.hpp                              :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <a_terminal.hpp>
#include <a_evaluable.hpp>
#include <stddef.h>

class TextConfigFile : public ATerminal, public AEvaluable
{
	private:
	protected:
	public:
		TextConfigFile(void);
		~TextConfigFile(void);

		const char	*what(void) const ;
		size_t		getTerminalSizeOnStr(const std::string & str) const;
		TextConfigFile*	clone(void) const;

		void	evaluate(Tree<AEvaluable*> *self);
};
