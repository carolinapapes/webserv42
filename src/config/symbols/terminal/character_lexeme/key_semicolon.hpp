/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_semicolon.hpp                                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <a_key_word.hpp>

class KeySemicolon : public AKeyWord
{
	private:
	protected:
	public:
		KeySemicolon(void);
		~KeySemicolon(void);

		const char	*what(void) const ;
		ATerminal*	clone(void) const;
};
