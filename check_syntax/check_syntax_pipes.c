#include "check_syntax.h"

// int	check_if_pipe_at_start(t_source *src, int *c, int is_start)	// MAYBE NOT NEEDED
// {
// //	src->inputline_size = strlen(src->inputline);
// 	*c = src->inputline[src->currpos];
// 	if (*c == '|' && is_start == 1)
// 		return (1);
// 	return (0);
// }



// RETURNS WITH THE NEW currpos ON THE
// LAST SPACE BEFORE THE NEXT CHAR
int	skip_allowed_chars(t_source *src)
{
	char c;

	if (src == NULL || src->inputline == NULL)
		return (1);
	c = peek_next_char(src);
	while (is_allowed_char(c) && c != '|' && c != ENDOFLINE) // ! maybe also is_space needed
	{
		// printf(GRN"   check pipes: loop, skip alphas ...\n"RES);
		//	get_next_char(src);		// probably can just be currpos++
		src->currpos++;
		c = peek_next_char(src);
	}
	return (0);
}



static int	skip_till_first_pipe(t_source *src, int *is_start, int *c)
{
	skip_white_spaces(src);
	*c = src->inputline[src->currpos + 1];
	//printf(GRN"stfp: pos%ld,  c[%c]\n"RES, src->currpos, *(c));
	if (*c == '|' && *is_start == 1)
	{
		//printf(GRN"stfp: A),  pos%ld,  c[%c]\n"RES, src->currpos, *c);
		return (1);
	}
	else if (*c == '"' || *c == '\'') //	 new jaka, skip all chars between quotes
	{
		//printf(GRN"    skip till pipe, pos %ld, c+0[%c]\n"RES, src->currpos, src->inputline[src->currpos+0]);
		if (src->inputline[src->currpos + 2] == *c) // added to handle "" EMPTY QUOTES
		{
			src->currpos += 3;
			//printf(GRN"       2nd quote found: pos %ld, c[%c][%d]\n"RES, src->currpos, src->inputline[src->currpos], src->inputline[src->currpos]);
			return (0);
		}
		src->currpos += 2;
		while (src->inputline[src->currpos] != *c)
			src->currpos++;
	}
	else if (is_allowed_char(*c) /*|| *c == '<' || *c == '>' */) // <> are alrady included, can erase
	{
		//printf(GRN"stfp: is allowed char, pos%ld, c[%c]\n"RES, src->currpos, *c);
		*is_start = 0;
		if (skip_allowed_chars(src) != 0) // if line == NULL
		{
			return (1);
		}
	}
	*c = src->inputline[src->currpos + 1];			// IS THIS NECESSARY ???
	return (0);
}



int	check_next_c_after_pipe(t_source *src)
{
	int	ret;

	ret = 0;
	src->currpos++;
	if (src->inputline[src->currpos + 1] == '|') // 2nd pipe ||
	{
		//printf("FOUND SECOND PIPE\n");
		//skip_white_spaces(src);
		//if (!is_allowed_char(src->inputline[src->currpos + 1]))
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



int check_pipes(t_source *src)
{
	int	c;
	int	is_start;

	//printf(GRN"check pipes\n"RES);
	src->currpos = -1;
	is_start = 1;
//	if (check_if_pipe_at_start(src, &c, is_start) != 0)	maybe not needed
//		return (1);
	while (src->currpos < src->inputline_size - 1) // -1 because it looks one ahead
	{
		if (skip_till_first_pipe(src, &is_start, &c) != 0)
			return  (1);
		if (c == '|')
		{
			if (check_next_c_after_pipe(src) != 0)
				return (1);
		}
	}
	//c = src->inputline[src->currpos];	// IS THIS NECESSARY ???
	src->currpos = 0;
	return (0);
}
