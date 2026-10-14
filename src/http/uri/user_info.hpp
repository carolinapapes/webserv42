/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   user_info.hpp                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>


class UserInfo
{
	private:
		std::string	_text;
		std::string	_user;
		std::string	_password;
		int			_isPasswordSet;

		void	fillUserAndPassword(void);
		void	normalize(void);
	protected:
	public:
		UserInfo(void);
		UserInfo(std::string text);
		UserInfo(std::vector<std::string>::iterator &iter, std::vector<std::string>::iterator end);
		~UserInfo(void);

		std::string	getUser(void) const ;
		std::string	getPassword(void) const ;
};
