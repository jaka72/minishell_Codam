/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmurovec <jmurovec@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/01 10:42:48 by jmurovec      #+#    #+#                 */
/*   Updated: 2022/06/02 10:44:41 by jmurovec      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

//  BOTH SYNTAX AND MAKECOMMANDS NEED TO HAVE ACCESS TO 
//							FOLDER jaka_utils TO READ
//	THE FILE utils.c, WITH FUNCTIONS SUCH AS get_next_char()
void	print_err_msg(char *err_msg)
{
	//printf("\nMinishell: %s\n", err_msg);
	write(2, "\nMinishel: ", 11);
	write(2, err_msg, ft_strlen(err_msg));
	write(2, "\n", 1);
	g_status = SYNTAX_ERROR;
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
int	is_valid_filename_char(int c)
{
	if (c >= 33 && c <= 126)
	{
		if (c == '<' || c == '>' || c == '|')
			return (0); // 0 == invalid
	}
	return (1);	// 1== valid
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
		src->currpos++;
}
