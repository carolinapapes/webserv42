/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   server.hpp                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <webserv/intermediate_representation.hpp>
#include "server_response.hpp"
#include "request.hpp"

class Server {
public:

	Server(const t_server &config);
	Server(const Server &other);
	Server &operator=(const Server &other);
	~Server();

	Response		handleErrorRequest(const Request &request);
	Response       handleRequest(const Request &request);

	const std::vector<std::string>   &getServerName() const;
	t_server    _config;

private:

};
