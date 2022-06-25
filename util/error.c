/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 11:30:23 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

int	return_perr(int i, char *tx)
{
	write(2, "minishell: ", 11);
	write(2, tx, ft_strlen(tx));
	perror(" ");
	return (i);
}

int	return_errtx(int i, char *tx)
{
	write(2, "minishell: ", 11);
	write(2, tx, ft_strlen(tx));
	return (i);
}

int	err_return_num(int exit_code, char *tx)
{
	if (tx != NULL)
		perror(tx);
	return (exit_code);
}

void	print_err_msg(char *err_msg)
{
	write(2, "minishell: ", 11);
	write(2, err_msg, ft_strlen(err_msg));
	write(2, "\n ", 1);
}

void	msg_and_exit(char *err_msg, int exit_code)
{
	write(2, err_msg, ft_strlen(err_msg));
	exit (exit_code);
}
