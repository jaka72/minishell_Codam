#include "../minishell.h"

int	count_elems(char **arr)
{
	int	i;

	if (arr == NULL)
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	is_allowed_char(int c)
{
	if (c >= 33 && c <= 126)
	{	
		return (1);
	}
	return (0);
}
// IF IT FINDS \ ) ( OR ; 
// IT ALREADY EXITS IN CHECK FOR QUOTES
///// PROBABLY HERE NOT NEEDED TO CHECK FOR INVALID  CHARS \ ( )
///// BECAUSE IF THEY APPEAR INSIDE QUOTES, THEY SHOULD BE VALID
///// IT IS ALREADY CHECKED WHEN CHECKING FOR CORRECT "QUOTES"
// 1 == valid, 0== invalid

int	is_valid_filename_char(int c)
{
	if (c >= 33 && c <= 126)
	{
		if (c == '<' || c == '>' || c == '|')
			return (0);
	}
	return (1);
}
// HERE, IF I BLOCK THE if, THEN I CORRUPT PIPES: ls | wc
// 0 == invalid
// 1 == valid

char	peek_next_char(t_source *src)
{
	long	pos;

	if (!src || !src->inputline)
	{
		return (NOINPUT);
	}
	pos = src->currpos;
	pos++;
	if (pos >= src->inputline_size)
	{
		return (ENDOFLINE);
	}
	return (src->inputline[pos]);
}

void	skip_white_spaces(t_source *src)
{
	char	c;

	if (src == NULL || src->inputline == NULL)
		return ;
	c = peek_next_char(src);
	while (c != ENDOFLINE && (isspace(c)))
	{
		src->currpos++;
		c = peek_next_char(src);
	}
}
// RETURNS THE NEW currpos ON THE
// LAST WHITE SPACE BEFORE THE NEXT CHAR
