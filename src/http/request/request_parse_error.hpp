/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parse_error.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 23:50:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/14 23:50:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdexcept>
#include <string>

class RequestParseError : public std::runtime_error
{
    private:
        int _errorCode;

    public:
        RequestParseError(int errorCode, const std::string &message)
            : std::runtime_error(message), _errorCode(errorCode) {}

        int getErrorCode() const { return _errorCode; }
};
