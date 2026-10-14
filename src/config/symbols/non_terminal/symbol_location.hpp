/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   symbol_location.hpp                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>

#include <a_terminal.hpp>
#include <a_non_terminal.hpp>

#include <webserv/intermediate_representation.hpp>

#include <symbol_limit_except.hpp>
#include <symbol_return.hpp>
#include <symbol_root.hpp>
#include <symbol_auto_index.hpp>
#include <symbol_index.hpp>

typedef enum
{
	LOCATION_RETURN,
	LOCATION_ROOT,
	LOCATION_INDEX,
	LOCATION_AUTOINDEX,
	LOCATION_LIMITEXCEPT,
	LOCATION_SUBLOCATION,
}	t_loctation_sub_directives;

class SymbolLocation : public ANonTerminal
{
	private:
		//	the first element will allways refer to the route of this instance of location.
		std::vector<t_route>	_routes;
		std::vector<int>		_bit_fields_set_directives;

		int		set_up_location_directive(AEvaluable* directive);
		int		set_up_location(Tree<AEvaluable*> *self);

		void	set_route_defaults(void);
		void	set_up_location_directive_return(SymbolReturn *directive);
		void	set_up_location_directive_root(SymbolRoot *directive);
		void	set_up_location_directive_index(SymbolIndex *directive);
		void	set_up_location_directive_autoindex(SymbolAutoIndex *directive);
		void	set_up_location_directive_limitexcept(SymbolLimitExcept *directive);

	protected:
	public:
		SymbolLocation(void);
		~SymbolLocation(void);

		const char		*what(void) const;
		SymbolLocation	*clone(void) const;
		void			evaluate(Tree<AEvaluable*> *self);

		AParser	*getParser(void) const;

		std::vector<t_route>	getRoutes(void) const ;
		std::vector<int>		getBitFieldsSetDirectives(void) const ;
};
