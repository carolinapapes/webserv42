/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ir_printing_functions.hpp                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <webserv/intermediate_representation.hpp>

#include <string>
#include <sys/socket.h>

std::string	getHTTPMethodSring(t_http_method method);
std::string	getResponseTypeSring(t_response_type type);

void	printRoute(t_route route);

void	printErrorPage(t_error_page error_page);

void	printSocket(struct sockaddr_storage);

void	printClientMaxBodySize(size_t size);

void	printServerName(std::vector<std::string>	name);

void	printServer(t_server server);
