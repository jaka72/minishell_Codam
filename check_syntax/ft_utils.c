#include "syntax_header.h"

int	print_err_msg(t_source *src, char *err_msg)
{
	printf("\nMinishell: %s\n", err_msg);
	return (SYNTAX_ERROR);
}



// IF IT FINDS \ ) ( OR ; 
// IT ALREADY EXITS IN CHECK FOR QUOTES
int	is_allowed_char(int c)
{
	if (c >= 33 && c <= 126)
	{	
		if (c == '(' || c == ')' || c == ';' || c == '\\')
			return (0); // 0 == error
		return (1);		// 1 == valid
	}
	else
		return (0);
}



char get_next_char(t_source *src)
{
	if (!src || !src->inputline)
	{
		return (NOINPUT);
	}
	src->currpos++;
	if (src->currpos >= src->inputline_size)
	{
		src->currpos = src->inputline_size;
		return ENDOFLINE;
	}
	return src->inputline[src->currpos];
}


char peek_next_char(t_source *src)
{
	long	pos;
	
	if (!src || !src->inputline)
	{
		return NOINPUT;
	}
	pos = src->currpos;

	pos++;
	if (pos >= src->inputline_size)
	{
		return ENDOFLINE;
	}
	return src->inputline[pos];
}


// RETURNS THE NEW currpos ON THE
// LAST WHITE SPACE BEFORE THE NEXT CHAR
void skip_white_spaces(t_source *src)
{
	char c;

	if (src == NULL || src->inputline == NULL)
		return;

	while (((c = peek_next_char(src)) != ENDOFLINE) && (isspace(c)))
		get_next_char(src);
}
