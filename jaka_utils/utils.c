/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmurovec <jmurovec@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/01 10:42:48 by jmurovec      #+#    #+#                 */
/*   Updated: 2022/06/06 13:12:21 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

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

// IF IT FINDS \ ) ( OR ; 
// IT ALREADY EXITS IN CHECK FOR QUOTES
///// PROBABLY HERE NOT NEEDED TO CHECK FOR INVALID  CHARS \ ( )
///// BECAUSE IF THEY APPEAR INSIDE QUOTES, THEY SHOULD BE VALID
///// IT IS ALREADY CHECKED WHEN CHECKING FOR CORRECT "QUOTES"
// 1 == valid, 0== invalid
int	is_allowed_char(int c)
{
	if (c >= 33 && c <= 126)
	{	
		return (1);
	}
	return (0);
}

// HERE, IF I BLOCK THE if, THEN I CORRUPT PIPES: ls | wc
// 0 == invalid
// 1 == valid
int	is_valid_filename_char(int c)
{
	if (c >= 33 && c <= 126)
	{
		if (c == '<' || c == '>' || c == '|')
			return (0);
	}
	return (1);
}

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

// RETURNS THE NEW currpos ON THE
// LAST WHITE SPACE BEFORE THE NEXT CHAR
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
// void skip_white_spaces(t_source *src)
// {
// 	char c;

// 	if (src == NULL || src->inputline == NULL)
// 		return;
// 	while (((c = peek_next_char(src)) != ENDOFLINE) && (isspace(c)))
// 		src->currpos++;
// }
