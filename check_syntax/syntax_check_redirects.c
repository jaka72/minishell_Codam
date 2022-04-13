#include "syntax_header.h"


void skip_alphas_spaces_pipes(t_source *src)
{
	char c;

	if (src == NULL || src->inputline == NULL)
		return;

	while (((c = peek_next_char(src)) != ENDOFLINE)
		&& (is_allowed_char(c) || ft_isspace(c) || c == '|')
		&& c != '<' && c != '>')
		get_next_char(src);
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



int	is_second_arrow(t_source *src, int *c, int arrow)
{
	int	ret;

	ret = 0;
	if (arrow != src->inputline[src->currpos + 1])		// <> or ><
		ret = 1;
	if (ft_isspace(src->inputline[src->currpos + 2]))	// < space
	{
		src->currpos += 2;
		skip_white_spaces(src);
		if (src->inputline[src->currpos + 1] == '<' || src->inputline[src->currpos + 1] == '>')	//  <<  < 
			ret = 1;
		else if (ft_isalpha(src->inputline[src->currpos + 1]))	//  <<  abc
		{
			src->currpos +=2;
			*c = src->inputline[src->currpos];
		}
	}
	else if (src->inputline[src->currpos + 2] == '\0')	// <<0 
		ret = 1;
	else if (src->inputline[src->currpos + 2] == arrow)	// <<<0 
		ret = 1;
	else if (ft_isalpha(src->inputline[src->currpos + 2]))	//  <  abc
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
	if (src->inputline[src->currpos + 1] == '>' || src->inputline[src->currpos + 1] == '<')
		ret = 1;
	if (src->inputline[src->currpos + 1] == '|')
		ret = 1;
	else if (src->inputline[src->currpos + 1] == '\0')
		ret = 1;
	else if (ft_isalpha(src->inputline[src->currpos + 1]))	//  <  abc
		*c = src->inputline[src->currpos];
	if (ret != 0)
		return (1);
	return (0);
}


int	check_redirects(t_source *src)
{
	int	c;
	int	arrow;

	c = src->inputline[src->currpos];
	while (src->currpos <= src->inputline_size)
	{
		if (c == '<' || c == '>')		// < at start
		{
			arrow = c;
			if (ft_isalpha(src->inputline[src->currpos + 1]))
			{
				src->currpos++;
				c = src->inputline[src->currpos];
			}
			else if (src->inputline[src->currpos + 1] == '<'
				|| src->inputline[src->currpos + 1] == '>')	// << or <> at start
			{
				if (is_second_arrow(src, &c, arrow) != 0)
					return (1);
			}
			else if (src->inputline[src->currpos + 1] == ' ') // <...
			{
				if (c == arrow)					// < at start
				{
					if (is_space_after_arrow(src, &c) != 0)
						return (1);
				}
			}
			else if (src->inputline[src->currpos + 1] == '\0')	// <0
				return (1);
		}
		if ((c == ' ' || is_allowed_char(c) || c == '|') /*&& c != '<' && c != '>'*/)
			if (is_space_alpha_or_pipe(src, &c) == 1)
				return (0);
	}
	src->currpos = -1;
	return (0);
}
