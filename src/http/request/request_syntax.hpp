/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_syntax.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 00:00:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/15 00:00:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "control_data.hpp"
#include "headers.hpp"

// Shared line validation for the framing state machine; no socket I/O.
namespace RequestSyntax
{
    ControlData validateControlData(const std::string& line);
    void validateHeader(const std::string& line, Headers& headers);
}
