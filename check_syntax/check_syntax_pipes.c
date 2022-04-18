#include "check_syntax.h"

int	check_if_pipe_at_start(t_source *src, int *c, int is_start)
{
	src->inputline_size = strlen(src->inputline);
	*c = src->inputline[src->currpos];
	if (*c == '|' && is_start == 1)
		return (1);
	return (0);
}



// RETURNS WITH THE NEW currpos ON THE
// LAST SPACE BEFORE THE NEXT CHAR
int	skip_alphas_spaces_arrows(t_source *src)
{
	char c;

	if (src == NULL || src->inputline == NULL)
		return (1);
	while (((c = peek_next_char(src)) != ENDOFLINE)
		&& is_allowed_char(c) && c != '|') // ! maybe also is_space needed
	{
		// printf(GRN"   check pipes: loop, skip alphas ...\n"RES);
		get_next_char(src);
	}
	// printf(GRN"      return ...\n"RES);
	return (0);
}



static int	skip_till_first_pipe(t_source *src, int *is_start, int *c)
{
	skip_white_spaces(src);	
	*c = src->inputline[src->currpos + 1];
	if (*c == '|' && *is_start == 1)
		return (1);
	else if (is_allowed_char(*c) || *c == '<' || *c == '>') // <> are alrady included, can erase
	{
		*is_start = 0;
		if (skip_alphas_spaces_arrows(src) != 0) // if line == NULL
			return (1);
	}
	*c = src->inputline[src->currpos + 1];
	//printf(GRN"   end skip till first pipe:\n"RES);
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

	is_start = 1;
	if (check_if_pipe_at_start(src, &c, is_start) != 0)
		return (1);
	while (src->currpos < src->inputline_size - 1) // maybe not - 1 ???
	{
		if (skip_till_first_pipe(src, &is_start, &c) != 0)
			return  (1);
		if (c == '|')
		{
			if (check_next_c_after_pipe(src) != 0)
				return (1);
		}
		//printf(GRN"      check pipes: loop ...\n"RES);

	}
	c = src->inputline[src->currpos];	
	src->currpos = 0;
	return (0);
}
