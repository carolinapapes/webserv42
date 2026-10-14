/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   server_manager.hpp                                :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

 #pragma once

#include <vector>
#include <webserv/intermediate_representation.hpp>
#include "server.hpp"
#include "server_response.hpp"
#include "request.hpp"

class ServerManager {
public:
    ServerManager();
    ~ServerManager();

    void addServer(const t_server& config);
    Response handleRequest(const Request& request);
    Response handleErrorRequest(const Request& request);

private:
    std::vector<Server*> servers;

    Server* findServer(const Request& request);
};
