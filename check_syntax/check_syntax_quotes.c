/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_syntax_quotes.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/01 18:22:44 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/01 18:23:12 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "check_syntax.h"

int	check_unsuported_chars(t_source *src)
{
	int	c;

	c = src->inputline[src->currpos];
	if (c == ';' || c == '(' || c == ')')
	{
		return (1);
	}
	return (0);
}

int	skip_till_end_quote(t_source *src, int c_open)
{
	int	c_curr;

	if (src->inputline[src->currpos + 1] == c_open)
	{
		src->currpos++;
		return (0);
	}
	src->currpos++;
	c_curr = src->inputline[src->currpos];
	while (c_curr != c_open && c_curr != '\0')
	{
		src->currpos++;
		c_curr = src->inputline[src->currpos];
	}
	if (c_curr != c_open)
		return (1);
	return (0);
}

int	check_quotes(t_source *src)
{
	int	c;

	c = src->inputline[src->currpos];
	while (c)
	{
		if (check_unsuported_chars(src) != 0)
			return (1);
		if (c == '"' || c == '\'')
		{
			if (skip_till_end_quote(src, c) != 0)
				return (2);
		}
		src->currpos++;
		c = src->inputline[src->currpos];
	}
	src->currpos = 0;
	return (0);
}
