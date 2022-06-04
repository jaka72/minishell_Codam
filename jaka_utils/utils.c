/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmurovec <jmurovec@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/01 10:42:48 by jmurovec      #+#    #+#                 */
/*   Updated: 2022/06/04 22:02:36 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"



int	count_elems(char **arr)
{
	int	i;

	//printf(GRN"Start count infiles\n"RES);
	if (arr == NULL)
		return (0);

	//printf(GRN"    arr is not NULL\n"RES);
	
	i = 0;
	while (arr[i])
		i++;
	//printf(GRN"   counted: %d\n"RES, i);
	return (i);
}



// int	count_args(t_cmd *cmd)	// THIS CAN BE REWRITTEN, SEE count_infiles()
int	count_args(char **arr)	// THIS CAN BE REWRITTEN, SEE count_infiles()
{
	int	i;

	//printf(GRN"Start count args "RES);
	if (arr == NULL)
		return (0);

	i = 0;
	while (arr[i])
		i++;
	//printf(GRN"   counted: %d\n"RES, i);
	return (i);
}




//  BOTH SYNTAX AND MAKECOMMANDS NEED TO HAVE ACCESS TO 
//							FOLDER jaka_utils TO READ
//	THE FILE utils.c, WITH FUNCTIONS SUCH AS get_next_char()
void	print_err_msg(char *err_msg)
{
	// JUST FOR THE TESTER
	printf("\nMinishell: %s\n", err_msg);  // !!! CHANGE BACK TO write()
	
	// write(2, "\nMinishel: ", 11);
	// write(2, err_msg, ft_strlen(err_msg));
	// write(2, "\n", 1);
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
