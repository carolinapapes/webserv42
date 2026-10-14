/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   multy_parse_exception.hpp                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <parse_exception.hpp>

class	multy_parse_exception
{
	private:
		std::string						_errorMsg;
		std::vector<parse_exception>	_errorVector;
	protected:
	public:
		multy_parse_exception(void);
		multy_parse_exception(parse_exception exception);
		multy_parse_exception(std::vector<parse_exception> exceptionVector);

		void	addException(parse_exception exception);
		void	makeErrorMsg(	const std::string & fileName, \
								const std::vector<std::string> & fileContent);
		std::string	what(void) const ;
};
