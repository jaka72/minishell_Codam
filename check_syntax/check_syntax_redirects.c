/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_syntax_redirects.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/01 18:41:52 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/01 18:46:18 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "check_syntax.h"

// Handles cases: <>,  ><, <<space,  <<0,  <<0,  < abc
int	is_second_arrow(t_source *src, int *c, int arrow)
{
	int	ret;

	ret = 0;
	if (arrow != src->inputline[src->currpos + 1])
		ret = 1;
	if (ft_isspace(src->inputline[src->currpos + 2]))
		ret = check_char_after_space(src, c);
	else if (src->inputline[src->currpos + 2] == '\0')
		ret = 1;
	else if (src->inputline[src->currpos + 2] == arrow)
		ret = 1;
	else if (ft_isalpha(src->inputline[src->currpos + 2]))
	{
		src->currpos += 2;
		*c = src->inputline[src->currpos];
	}
	if (ret != 0)
		return (1);
	return (0);
}

int	is_space_after_arrow(t_source *src, int *c)
{
	int	ret;

	ret = 0;
	skip_white_spaces(src);
	if (src->inputline[src->currpos + 1] == '>'
		|| src->inputline[src->currpos + 1] == '<')
		ret = 1;
	if (src->inputline[src->currpos + 1] == '|')
		ret = 1;
	else if (src->inputline[src->currpos + 1] == '\0')
		ret = 1;
	else if (ft_isalpha(src->inputline[src->currpos + 1]))
		*c = src->inputline[src->currpos];
	if (ret != 0)
		return (1);
	return (0);
}

// Looks for cases at start: <,  <0,  <<.  <> 
int	check_chars_after_arrow(t_source *src, int *c, int arrow)
{
	if (ft_isalpha(src->inputline[src->currpos + 1]))
	{
		src->currpos++;
		*c = src->inputline[src->currpos];
	}
	else if (src->inputline[src->currpos + 1] == '<'
		|| src->inputline[src->currpos + 1] == '>')
	{
		if (is_second_arrow(src, c, arrow) != 0)
			return (1);
	}
	else if (src->inputline[src->currpos + 1] == ' ')
	{
		if (*c == arrow)
		{
			if (is_space_after_arrow(src, c) != 0)
				return (1);
		}
	}
	else if (src->inputline[src->currpos + 1] == '\0')
		return (1);
	return (0);
}

// NOT SURE IF GOOD while <= MAYBE JUST <
int	check_redirects(t_source *src)
{
	int	c;
	int	arrow;

	c = src->inputline[src->currpos];
	while (src->currpos <= src->inputline_size)
	{
		if (c == '<' || c == '>')
		{
			arrow = c;
			if (check_chars_after_arrow(src, &c, arrow) != 0)
				return (1);
		}
		if (c == ' ' || is_allowed_char(c) || c == '|')
		{
			if (is_space_alpha_or_pipe(src, &c) == 1)
				return (0);
		}
	}
	src->currpos = -1;
	return (0);
}
