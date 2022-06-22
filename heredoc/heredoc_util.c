/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_util.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:13:25 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_pid_sig_hd(t_pid *pidinfo)
{
	pidinfo->status = 0;
	pidinfo->newpipe[0] = 0;
	pidinfo->newpipe[1] = 0;
	pidinfo->newpipe[2] = 0;
	pidinfo->temp_cmd = g_gl.start_cmd;
	signal(SIGINT, handle_sigint_hd);
	signal(SIGQUIT, handle_sigquit);
}

int	reset_fd_sig_close(int fd)
{
	reset_fd_sig();
	close(fd);
	return (-1);
}
