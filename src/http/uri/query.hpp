/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   query.hpp                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>

//	this are only the character that don't fall in any other group like ptc encoded or such.
#define PCHAR ":@"

class Query
{
	private:
		std::string	_text;

	protected:
	public:
		Query(void);
		Query(std::string text);
		Query(std::vector<std::string>::iterator &iter, std::vector<std::string>::iterator end);
		~Query(void);

		std::string	getText(void) const ;
};
