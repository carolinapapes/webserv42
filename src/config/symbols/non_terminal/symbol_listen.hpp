/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_listen.hpp                                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>

#include <webserv/intermediate_representation.hpp>

class SymbolListen : public ANonTerminal
{
	private:
		std::vector<struct sockaddr_storage>	_sockets;
	protected:
	public:
		SymbolListen(void);
		~SymbolListen(void);

		const char		*what(void) const;
		SymbolListen	*clone(void) const;
		void			evaluate(Tree<AEvaluable*> *self);

		AParser	*getParser(void) const;

		std::vector<struct sockaddr_storage>	getSockets(void) const ;
};
