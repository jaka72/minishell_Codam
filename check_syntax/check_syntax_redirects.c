#include "check_syntax.h"

int	is_second_arrow(t_source *src, int *c, int arrow)
{
	int	ret;

	ret = 0;
	if (arrow != src->inputline[src->currpos + 1])		// <> or ><
		ret = 1;
	if (ft_isspace(src->inputline[src->currpos + 2]))	// << space
		ret = check_char_after_space(src, c);
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


int	is_space_after_arrow(t_source *src, int *c/*, int arrow*/)
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
	else if (ft_isalpha(src->inputline[src->currpos + 1]))	//  <  abc
		*c = src->inputline[src->currpos];
	if (ret != 0)
		return (1);
	return (0);
}


int	check_chars_after_arrow(t_source *src, int *c, int arrow)
{
	if (ft_isalpha(src->inputline[src->currpos + 1]))
	{
		src->currpos++;
		*c = src->inputline[src->currpos];
	}
	else if (src->inputline[src->currpos + 1] == '<'
		|| src->inputline[src->currpos + 1] == '>')	// << or <> at start
	{
		if (is_second_arrow(src, c, arrow) != 0)
			return (1);
	}
	else if (src->inputline[src->currpos + 1] == ' ') // <...
	{
		if (*c == arrow)					// < at start
		{
			if (is_space_after_arrow(src, c/*, arrow*/) != 0)
				return (1);
		}
	}
	else if (src->inputline[src->currpos + 1] == '\0')	// <0
		return (1);
	return (0);
}



int	check_redirects(t_source *src)
{
	int	c;
	int	arrow;

	//printf(BLU"Start check redirs\n"RES);
	c = src->inputline[src->currpos];
	while (src->currpos <= src->inputline_size) // NOT SURE IF GOOD <= MAYBE JUST <
	{
		//printf(BLU"   loop\n"RES);
		if (c == '<' || c == '>')		// < at start
		{
			arrow = c;
			if (check_chars_after_arrow(src, &c, arrow) != 0)
				return (1);
		}
		//if ((c == ' ' || is_allowed_char(c) || c == '|') /*&& c != '<' && c != '>'*/)
		if (c == ' ' || is_allowed_char(c) || c == '|')
		{
			// if (c == '"' || c == '\'') //	 new jaka, skip all chars between quotes
			// {
			// 	src->currpos++;
			// 	// while (src->inputline[src->currpos] != '"')
			// 	while (src->inputline[src->currpos] != c)
			// 	{
			// 		printf(BLU" %c"RES, c);
			// 		src->currpos++;
			// 	}
			// 	src->currpos++;
			// 	c = src->inputline[src->currpos];
			// }
			if (is_space_alpha_or_pipe(src, &c) == 1)
				return (0);
		}
	}
	src->currpos = -1;
	return (0);
}
