/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   config_file_terminal_factory.cpp                  :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <config_file_terminal_factory.hpp>

#include <key_word_server.hpp>
#include <key_word_server_name.hpp>
#include <key_word_listen.hpp>
#include <key_word_client_max_body_size.hpp>
#include <key_word_error_page.hpp>
#include <key_word_location.hpp>
#include <key_word_return.hpp>
#include <key_word_limit_exept.hpp>
#include <key_word_root.hpp>
#include <key_word_auto_index.hpp>
#include <key_word_index.hpp>

#include <key_left_curly_bracket.hpp>
#include <key_right_curly_bracket.hpp>
#include <key_semicolon.hpp>

#include <text_config_file.hpp>
#include <number.hpp>

#include <white_space.hpp>

ConfigFileTerminalFactory::ConfigFileTerminalFactory(void)
{
	this->resetIndex();
	this->addTerminalToFactory(new KeyWordServer());
	this->addTerminalToFactory(new KeyWordServerName());
	this->addTerminalToFactory(new KeyWordListen());
	this->addTerminalToFactory(new KeyWordClientMaxBodySize());
	this->addTerminalToFactory(new KeyWordErrorPage());
	this->addTerminalToFactory(new KeyWordLocation());
	this->addTerminalToFactory(new KeyWordReturn());
	this->addTerminalToFactory(new KeyWordLimitExept());
	this->addTerminalToFactory(new KeyWordRoot());
	this->addTerminalToFactory(new KeyWordAutoIndex());
	this->addTerminalToFactory(new KeyWordIndex());

	this->addTerminalToFactory(new KeyLeftCurlyBracket());
	this->addTerminalToFactory(new KeyRightCurlyBracket());
	this->addTerminalToFactory(new KeySemicolon());

	this->addTerminalToFactory(new Number());
	this->addTerminalToFactory(new TextConfigFile());

	this->addTerminalToFactory(new WhiteSpace());
}
