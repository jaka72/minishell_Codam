#include "../minishell.h"

static	int	skip_allowed_chars(t_source *src)
{
	char	c;

	if (src == NULL || src->inputline == NULL)
		return (1);
	c = peek_next_char(src);
	while (is_allowed_char(c) && c != '|' && c != ENDOFLINE)
	{
		src->currpos++;
		c = peek_next_char(src);
	}
	return (0);
}
// RETURNS WITH THE NEW currpos ON THE
// LAST SPACE BEFORE THE NEXT CHAR

static int	skip_till_first_pipe(t_source *src, int *is_start, int *c)
{
	skip_white_spaces(src);
	*c = src->inputline[src->currpos + 1];
	if (*c == '|' && *is_start == 1)
		return (1);
	else if (*c == '"' || *c == '\'')
	{
		if (src->inputline[src->currpos + 2] == *c)
		{
			src->currpos += 3;
			return (0);
		}
		src->currpos += 2;
		while (src->inputline[src->currpos] != *c)
			src->currpos++;
	}
	else if (is_allowed_char(*c))
	{
		*is_start = 0;
		if (skip_allowed_chars(src) != 0)
			return (1);
	}
	*c = src->inputline[src->currpos + 1];
	return (0);
}

static	int	check_next_c_after_pipe(t_source *src)
{
	int	ret;

	ret = 0;
	src->currpos++;
	if (src->inputline[src->currpos + 1] == '|')
	{
		ret = 1;
	}
	else if (src->inputline[src->currpos + 1] == '\0')
		ret = 1;
	else if (ft_isspace(src->inputline[src->currpos + 1]))
	{
		skip_white_spaces(src);
		if (src->inputline[src->currpos + 1] == '\0')
			ret = 1;
		if (src->inputline[src->currpos + 1] == '|')
			ret = 1;
	}
	if (ret == 1)
		return (1);
	return (0);
}

int	check_pipes(t_source *src)
{
	int	c;
	int	is_start;

	src->currpos = -1;
	is_start = 1;
	while (src->currpos < src->inputline_size - 1)
	{
		if (skip_till_first_pipe(src, &is_start, &c) != 0)
			return (1);
		if (c == '|')
		{
			if (check_next_c_after_pipe(src) != 0)
				return (1);
		}
	}
	src->currpos = 0;
	return (0);
}
// while is less than -1, because it looks one char ahead
