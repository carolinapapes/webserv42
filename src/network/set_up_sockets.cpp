/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   set_up_sockets.cpp                                :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <set_up_sockets.hpp>
#include "epoll_connection_manager.hpp"
#include "socket.hpp"
#include <iostream>
#include <set>

#include <webserv/intermediate_representation.hpp>

#include <cstring>

static std::vector<struct sockaddr_storage>	add_socket(std::vector<struct sockaddr_storage> sockets, struct sockaddr_storage socket)
{
	size_t	socket_size;
	if (socket.ss_family == AF_INET)
		socket_size = sizeof(sockaddr_in);
	else
		socket_size = sizeof(sockaddr_in6);

	size_t	i = 0;
	while (i < sockets.size())
	{
		if (!std::memcmp(&sockets[i], &socket, socket_size))
			return (sockets);
		i++;
	}
	sockets.push_back(socket);
	return (sockets);
}

static std::vector<struct sockaddr_storage>	add_sockets(std::vector<struct sockaddr_storage> sockets, std::vector<struct sockaddr_storage> new_sockets)
{
	size_t	i = 0;
	while (i < new_sockets.size())
	{
		sockets = add_socket(sockets, new_sockets[i]);
		i++;
	}
	return (sockets);
}

static std::vector<struct sockaddr_storage>	get_distict_sockets(std::vector<t_server> servers)
{
	std::vector<struct sockaddr_storage> diferent_sockets;

	size_t	i = 0;
	while (i < servers.size())
	{
		diferent_sockets = add_sockets(diferent_sockets, servers[i].socket);
		i++;
	}
	return (diferent_sockets);
}

std::map<int, Socket *>	set_up_sockets(std::vector<t_server> servers)
{
	std::vector<struct sockaddr_storage> diferent_sockets = get_distict_sockets(servers);

	std::map<int, Socket*>	listeningSockets;
	size_t	i = 0;
	while (i < diferent_sockets.size())
	{
		Socket	*socket = new Socket(diferent_sockets[i]);
		listeningSockets[socket->getFd()] = socket;
		i++;
	}
	return (listeningSockets);
}
