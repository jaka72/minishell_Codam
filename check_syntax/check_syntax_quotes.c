#include "check_syntax.h"

int	check_unsuported_chars(t_source *src)
{
	int	c;

	c = src->inputline[src->currpos];
	//while (c != '\0')
	//{
		if (/* c == '\\' || */ c == ';' || c == '(' || c == ')')
		{
			// print_err_msg(MSG_UNSUPPORTED);
			return (1);
		}
	//	src->currpos++;
	//	c = src->inputline[src->currpos];
	//}
	//src->currpos = 0;
	return (0);
}


int	skip_till_end_quote(t_source *src, int c_open)
{
	int	c_curr;

	src->currpos++;
	c_curr = src->inputline[src->currpos];
	while (c_curr != c_open && c_curr != '\0')
	{
		//printf("b) skip\n");
		src->currpos++;
		c_curr = src->inputline[src->currpos];
	}
	if (c_curr != c_open)
		return (1);
	
	return (0);
}

int	check_quotes(t_source *src)
{
	//printf("\n"BLU"   a check quotes, currpos %ld\n"RES, src->currpos);
	printf(CYN"line [%s]\n"RES, src->inputline);

	int	c;


	c = src->inputline[src->currpos];
	while (src->currpos < src->inputline_size)
	{
		//printf("\n"BLU"   b check quotes, currpos %ld\n"RES, src->currpos);
		if (check_unsuported_chars(src) != 0)
			return (1);
		if (c == '"' || c == '\'')
		{
			//printf("\n"BLU"   Found a quote, start skipping \n"RES);
			if (skip_till_end_quote(src, c) != 0)
				return (2);
		}
		//printf("a)  %ld[%c]\n", src->currpos, c);

		src->currpos++;
		c = src->inputline[src->currpos];
	}

	//printf("a) END %ld[%c]\n", src->currpos, c);
	src->currpos = 0;
	return (0);
}
