/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   req_scanner.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 10:02:01 by capapes           #+#    #+#             */
/*   Updated: 2025/12/03 18:03:31 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "req_scanner.hpp"
#include <stdexcept>

ReqScanner::ReqScanner(const std::string& raw)
	: _raw(raw), _pos(0)
{
}

ReqScanner::~ReqScanner() {}

bool ReqScanner::isEndPos() const
{
	return _pos == _raw.size();
}

bool ReqScanner::getField(const std::string& delimiter, std::string& field)
{
	if (delimiter.empty())
		throw std::invalid_argument("empty delimiter; use getRemaining()");
	const size_t end = _raw.find(delimiter, _pos);
	if (end == std::string::npos)
	{
		field.clear();
		return false;
	}
	const size_t next = end + delimiter.size();
	field = _raw.substr(_pos, end - _pos);
	_pos = next;
	return true;
}

std::string ReqScanner::getRemaining()
{
	const std::string remaining = _raw.substr(_pos);
	_pos = _raw.size();
	return remaining;
}
