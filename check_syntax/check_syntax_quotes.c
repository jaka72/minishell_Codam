/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_syntax_quotes.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:12:43 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	check_unsuported_chars(t_source *src)
{
	int	c;

	c = src->inputline[src->currpos];
	if (c == ';' || c == '(' || c == ')')
	{
		return (1);
	}
	return (0);
}

static	int	skip_till_end_quote(t_source *src, int c_open)
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
