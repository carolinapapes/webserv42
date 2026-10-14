/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   configuration_parser.hpp                          :+:      :+:    :+:    */
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

#include <a_symbol.hpp>
#include <a_terminal.hpp>
#include <a_evaluable.hpp>
#include <args.hpp>
#include <tree.hpp>

#include <webserv/intermediate_representation.hpp>

//	this class in responsible for transforming the file in the IR for the web server to execute.
class ConfigurationParser
{
	private:
		std::string							_configFileName;
		std::vector<std::string>			_configFileContent;
		std::vector<ATerminal*>				_terminalList;
		Tree<AEvaluable*>					*_AST;

		void	addTerminalsToList(std::vector<ATerminal*> line);
		void	printTerminalList(void) const ;
		void	printAST(void) const ;

	protected:
	public:
		ConfigurationParser(const Args & args);
		~ConfigurationParser(void);
		void	readFile(void);
		void	scanning(void);
		void	parsing(void);
		void	analysis(void);
		std::vector<t_server>	getServers(void) const ;
};
