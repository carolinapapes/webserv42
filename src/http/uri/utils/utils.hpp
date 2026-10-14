/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   utils.hpp                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

#define digit_char	"0123456789abcdefghijklmnopqrstuvwxyz"


int	stoi(std::string num_str, size_t *pos = 0, int base = 10);

int	stricter_unsigned_stoi(std::string num_str, size_t *pos = 0, int base = 10);
