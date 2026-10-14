/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   tokenize.cpp                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <tokenize.hpp>

#include <vector>
#include <string>

std::vector<std::string>	tokenize(std::string text, std::string delimiters)
{
	size_t	start = 0;
	size_t	end = text.find_first_of(delimiters, start);
	if (end == start)
		end++;

	std::vector<std::string>	tokens;
	while (end < text.size())
	{
		tokens.push_back(text.substr(start, end - start));

		start = end;
		end = text.find_first_of(delimiters, start);
		if (end == start)
			end++;
	}
	tokens.push_back(text.substr(start, end - start));
	return (tokens);
}
