/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_index.hpp                                :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <a_terminal.hpp>
#include <a_key_word.hpp>

class KeyWordIndex : public AKeyWord
{
	private:
	protected:
	public:
		KeyWordIndex(void);
		~KeyWordIndex(void);

		const char	*what(void) const ;
		ATerminal*	clone(void) const;
};
