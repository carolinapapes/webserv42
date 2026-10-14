/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_right_curly_bracket.hpp                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <a_key_word.hpp>

class KeyRightCurlyBracket : public AKeyWord
{
	private:
	protected:
	public:
		KeyRightCurlyBracket(void);
		~KeyRightCurlyBracket(void);

		const char	*what(void) const ;
		ATerminal*	clone(void) const;
};
