/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_word_server_name.hpp                          :+:      :+:    :+:    */
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

class KeyWordServerName : public AKeyWord
{
	private:
	protected:
	public:
		KeyWordServerName(void);
		~KeyWordServerName(void);


		const char	*what(void) const ;
		ATerminal* clone() const;
};
