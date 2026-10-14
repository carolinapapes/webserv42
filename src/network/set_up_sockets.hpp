/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   set_up_sockets.hpp                                :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "epoll_connection_manager.hpp"
#include "socket.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <webserv/intermediate_representation.hpp>

std::map<int, Socket *>	set_up_sockets(std::vector<t_server> servers);
