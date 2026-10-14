/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parse_errno_msg.hpp                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//		note to self: please remember to put the commas were they are needed. it's not ben the
//	first or the second or the third.

const static char	*g_parse_errno_msg_type[] = {\
	"something went wrong.",	\
	"unrecognized character.",	\
	"quote sign not closed.",	\
	"unrecognized symbol.",	\
	"all symbols consumed whitout finishing the ast.",	\
	"." \
};
