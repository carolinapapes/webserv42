/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatahere <tatahere@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:35:18 by tatahere          #+#    #+#             */
/*   Updated: 2025/11/06 14:26:49 by tatahere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <iostream>
#include <map>
#include <vector>

#include <configuration_parser.hpp>
#include <args.hpp>
#include <webserv/text_colors.hpp>
#include <multy_parse_exception.hpp>
#include <webserv/intermediate_representation.hpp>
#include <epoll_connection_manager.hpp>
#include <socket.hpp>

#include <set_up_sockets.hpp>

#include <server_manager.hpp>

int	main(int argc, char **argv)
{
	try
	{
		Args				args(argc, argv);
		ConfigurationParser	parser(args);

		parser.readFile();
		parser.scanning();
		parser.parsing();
		parser.analysis();

		std::vector<t_server>	servers = parser.getServers();

		ServerManager	server_manager;
		size_t	i = 0;
		while (i < servers.size())
		{
			server_manager.addServer(servers[i]);
			i++;
		}

		std::map<int, Socket *>	listeningSockets = set_up_sockets(servers);

        EpollConnectionManager manager(listeningSockets, server_manager);

		std::cout << "starting web server." << std::endl;
	}
	catch (multy_parse_exception & e)
	{
		std::cerr << e.what() << std::flush;
		return (1);
	}
	catch (const std::exception & e)
	{
		std::cerr << "THROW" << std::endl;
		if (argc > 0)
			std::cerr << RED << e.what() << RESET << std::endl;
		else
			std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
