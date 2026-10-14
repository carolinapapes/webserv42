/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   text_config_file.cpp                              :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <sstream>

#include <string>
#include <cctype>
#include <cstdlib>

#include <text_config_file.hpp>

TextConfigFile::TextConfigFile(void) : ATerminal()
{
}

TextConfigFile::~TextConfigFile(void)
{
}

TextConfigFile* TextConfigFile::clone() const
{
	return new TextConfigFile(*this);
}

//	safe characters that arent used in control or as separators.
static int	is_safe(int ch)
{
	if (!std::isprint(ch))
		return (0);
	if (ch == ' ' || ch == '{' || ch == '}' || ch == ';' || ch == '"')
		return (0);
	return (1);
}

static size_t	get_quoted_section_len(char *str)
{
	size_t	i = 0;

	if (str[i] == '"')
		i++;
	else
		return (0);
	while (str[i] >= ' ' && str[i] <= '~' && str[i] != '"')
	{
		i++;
	}
	if (str[i] == '"')
		i++;
	else
		return (0);
	return (i);
}

size_t	TextConfigFile::getTerminalSizeOnStr(const std::string & str) const
{
	size_t	i = 0;
	size_t	quoted_section_len = 0;
	char	*c_str = (char *) str.c_str();

	while (is_safe(c_str[i]) || c_str[i] == '"')
	{
		quoted_section_len = get_quoted_section_len(c_str);
		if (c_str[i] == '"' && quoted_section_len == 0)
			break ;
		if (quoted_section_len != 0)
			i += quoted_section_len;
		else
			i++;
	}
	return (i);
}

void	TextConfigFile::evaluate(Tree<AEvaluable*> *self)
{
	(void) self;
}

const char	*TextConfigFile::what(void) const
{
	return ("text");
}
