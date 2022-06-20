#include "../minishell.h"
#define ENDOFLINE	(-1)

static	void	skip_alphas_spaces_pipes(t_source *src)
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
//  Checking cases like: 	<<  < 	and  <<  abc
