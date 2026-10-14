/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   config_file_lexer.cpp                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <config_file_lexer.hpp>
#include <config_file_terminal_factory.hpp>

ConfigFileLexer::ConfigFileLexer(void)
{
	this->_factory = new ConfigFileTerminalFactory();
}
