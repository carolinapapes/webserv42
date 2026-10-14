/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_listen.hpp                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <a_key_word.hpp>

class KeyWordListen : public AKeyWord
{
	private:
	protected:
	public:
		KeyWordListen(void);
		~KeyWordListen(void);

		const char	*what(void) const ;
		ATerminal* clone() const;
};
