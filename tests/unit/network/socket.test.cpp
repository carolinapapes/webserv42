/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   socket.test.cpp                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <socket.hpp>
#include <set_up_sockets.hpp>
#include "../../support/test_assertions.hpp"

#include <cerrno>
#include <cstring>

int main()
{
    TestSuite suite("Listening sockets");
    struct sockaddr_storage address;
    std::memset(&address, 0, sizeof(address));
    struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in *>(&address);
    ipv4->sin_family = AF_INET;
    ipv4->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ipv4->sin_port = 0;

    int fd;
    {
        Socket socket(address);
        fd = socket.getFd();
        suite.check(fd >= 0, "socket descriptor created");
        const int flags = fcntl(fd, F_GETFL, 0);
        suite.check(flags != -1 && (flags & O_NONBLOCK) != 0, "listener is nonblocking");
        int listening = 0;
        socklen_t size = sizeof(listening);
        suite.check(getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &listening, &size) == 0 && listening == 1,
                    "socket is listening");
    }
    errno = 0;
    suite.check(fcntl(fd, F_GETFD) == -1 && errno == EBADF, "destructor closes descriptor");

    std::vector<t_server> servers(2);
    servers[0].socket.push_back(address);
    servers[1].socket.push_back(address);
    std::map<int, Socket *> sockets = set_up_sockets(servers);
    suite.check(sockets.size() == 1, "duplicate listen configurations share one socket");
    for (std::map<int, Socket *>::iterator it = sockets.begin(); it != sockets.end(); ++it)
        delete it->second;
    return suite.finish();
}
