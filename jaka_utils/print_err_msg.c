/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_err_msg.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/06 13:06:34 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/06 13:13:33 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

//  BOTH SYNTAX AND MAKECOMMANDS NEED TO HAVE ACCESS TO 
//							FOLDER jaka_utils TO READ
//	THE FILE utils.c, WITH FUNCTIONS SUCH AS get_next_char()
//	 printf is JUST FOR THE TESTER    // !!! CHANGE BACK TO write()
void	print_err_msg(char *err_msg)
{
	printf("\nMinishell: %s\n", err_msg);
	// write(2, "\nMinishel: ", 11);
	// write(2, err_msg, ft_strlen(err_msg));
	// write(2, "\n", 1);
	g_status = SYNTAX_ERROR;
}

void	msg_and_exit(char *err_msg, int exit_code)
{
	write(2, err_msg, ft_strlen(err_msg));
	exit (exit_code);
}
