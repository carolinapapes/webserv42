/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   fragment.hpp                                      :+:      :+:    :+:    */
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

class Fragment
{
	private:
		std::string	_text;

		void	checkFragmentText(void);
	protected:
	public:
		Fragment(void);
		Fragment(std::string text);
		Fragment(std::vector<std::string>::iterator &iter, std::vector<std::string>::iterator end);
		~Fragment(void);

		std::string	getText(void) const ;
};
