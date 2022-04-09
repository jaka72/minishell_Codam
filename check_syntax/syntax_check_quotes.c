#include "syntax_header.h"


int check_quotes(t_source *src)
{
	int	c;
	int	nr_quotes;

	nr_quotes = 0;
	c = src->inputline[src->currpos];

	while (src->currpos < src->inputline_size)
	{
		if (c == '"')
			nr_quotes++;

		else if (c == '\\' || c == ';' || c == '(' || c == ')')
			print_err_msg(src, MSG_QUOTES);
		src->currpos++;
		c = src->inputline[src->currpos];
	}

	if (nr_quotes % 2 != 0)
		print_err_msg(src, MSG_QUOTES);

	src->currpos = 0;
	return (0);
}
