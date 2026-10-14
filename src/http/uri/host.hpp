/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   host.hpp                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>

#include <ip_literal.hpp>

typedef enum
{
	REG_NAME = 0 ,					//	REG_NAME is registered name.
	IP_LITERAL,
}	t_host_type;

class Host
{
	private:
		std::string	_text;

		t_host_type	_type;

		IpLiteral	_ip;

	protected:
	public:
		Host(void);
		Host(std::string text);
		Host(std::vector<std::string>::iterator &iter, std::vector<std::string>::iterator end);
		~Host(void);

		t_host_type	getType(void) const ;
		std::string	getRegName(void) const ;
		IpLiteral	&getIp(void);
};
