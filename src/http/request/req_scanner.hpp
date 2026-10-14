/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_scanner.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 09:58:11 by capapes           #+#    #+#             */
/*   Updated: 2025/12/03 18:03:02 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Request scanner
// Owns its input and extracts fields sequentially.
// Missing delimiters leave the position unchanged and return false.
// An extracted field may be empty; getRemaining() explicitly consumes the rest.

#pragma once
#include <string>

class ReqScanner
{
public:
	ReqScanner(const std::string& raw);
	~ReqScanner();

	bool		getField(const std::string& delimiter, std::string& field);
	std::string getRemaining();
	bool		isEndPos() const;

private:
	std::string 	_raw;
	size_t 			_pos;
};
