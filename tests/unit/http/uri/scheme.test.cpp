/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   scheme.test.cpp                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <scheme.hpp>

#include <string>
#include <vector>
#include <stdexcept>

// simple test with no scheme

int main(void)
{
	Scheme	test;

	if (test.getSchemeType() != scheme_UNOFFICIAL)
		return (1);

	return (0);
}
