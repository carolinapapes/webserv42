/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   white_space.hpp                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <a_terminal.hpp>

class WhiteSpace : public ATerminal
{
	private:
	protected:
	public:
		WhiteSpace(void);
		~WhiteSpace(void);

		const char	*what(void) const ;
		size_t		getTerminalSizeOnStr(const std::string & str) const;
		ATerminal*	clone(void) const;
};
