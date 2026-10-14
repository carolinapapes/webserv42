/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_return.hpp                               :+:      :+:    :+:    */
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

class KeyWordReturn : public AKeyWord
{
	private:
	protected:
	public:
		KeyWordReturn(void);
		~KeyWordReturn(void);

		const char	*what(void) const ;
		ATerminal* clone() const;
};
