/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   args.hpp                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Args
{
	private:
		std::string	_configFileName;
	protected:
	public:
		Args(int argc, char **argv);
		~Args(void);

		std::string	getConfigFileName(void) const ;
};
