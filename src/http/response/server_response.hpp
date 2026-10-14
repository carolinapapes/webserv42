/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   server_response.hpp                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sstream>

class ServerResponse {
public:
	int         status_code;
	std::string content_type;
	std::string body;
	std::string location; // optional Location header for redirects (empty when unused)


	ServerResponse();

	ServerResponse(int code, const std::string &type, const std::string &b);
	std::string to_string() const;
};

enum ResponseStatus {
	RESP_OK,
	RESP_ERR,
	RESP_CGI
};

struct Response {
	ResponseStatus status;
	ServerResponse sres;
	std::string    pathToCgi;
};

std::string get_status_message(int code);
