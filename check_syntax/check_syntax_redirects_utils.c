/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_syntax_redirects_utils.c                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/01 18:27:16 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/20 09:16:52 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include "check_syntax.h"
#include "../minishell.h"

void	skip_alphas_spaces_pipes(t_source *src)
{
	char	c;

	if (src == NULL || src->inputline == NULL)
		return ;
	c = peek_next_char(src);
	while (c != ENDOFLINE && (is_allowed_char(c) || ft_isspace(c) || c == '|')
		&& c != '<' && c != '>')
	{
		if (c == '"' || c == '\'')
		{
			if (src->inputline[src->currpos + 1] == c)
			{
				src->currpos += 1;
				return ;
			}
			src->currpos += 2;
			while (src->inputline[src->currpos] != c)
			{
				src->currpos++;
			}
			src->currpos++;
		}
		src->currpos++;
		c = peek_next_char(src);
	}
}

int	is_space_alpha_or_pipe(t_source *src, int *c)
{
	skip_alphas_spaces_pipes(src);
	if (src->inputline[src->currpos + 1] == '<'
		|| src->inputline[src->currpos + 1] == '>')
	{
		src->currpos++;
		*c = src->inputline[src->currpos];
	}
	else if (src->inputline[src->currpos + 1] == '\0')
		return (1);
	return (0);
}

//  Checking cases like: 	<<  < 	and  <<  abc
int	check_char_after_space(t_source *src, int *c)
{
	src->currpos += 2;
	skip_white_spaces(src);
	if (src->inputline[src->currpos + 1] == '<'
		|| src->inputline[src->currpos + 1] == '>'
		|| src->inputline[src->currpos + 1] == '\0')
		return (1);
	else if (ft_isalpha(src->inputline[src->currpos + 1]))
	{
		src->currpos += 2;
		*c = src->inputline[src->currpos];
	}
	return (0);
}
