/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connect_fd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:13:04 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	connect_fd(t_cmd *current)
{
	open_file_fd(current);
	if (current->fd_in > 0)
	{
		dup2(current->fd_in, 0);
		close(current->fd_in);
		current->fd_in = 0;
	}
	if (current->fd_out > 1)
	{
		dup2(current->fd_out, 1);
		close(current->fd_out);
		current->fd_out = 1;
	}
	return (0);
}

void	check_close_fd(int fdin, int fdout)
{
	if (fdin > 2)
		close(fdin);
	if (fdout > 2)
		close(fdout);
}

int	reset_fd_sig(t_util *st_base)
{
	int	fd0;
	int	fd1;

	fd0 = dup(st_base->ini_fd[0]);
	fd1 = dup(st_base->ini_fd[1]);
	dup2(fd0, 0);
	dup2(fd1, 1);
	close(fd0);
	close(fd1);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	return (1);
}
