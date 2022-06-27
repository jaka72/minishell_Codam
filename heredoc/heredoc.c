/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/27 06:37:35 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>

static int	read_heredoc(char *limiter, int fd_out, t_util *st_base)
{
	char	*str;

	str = readline("> ");
	while (str)
	{
		if ((ft_strncmp(str, limiter, ft_strlen(limiter)) == 0)
			&& ft_strlen(limiter) == ft_strlen(str))
		{
			free(str);
			break ;
		}	
		str = check_expand_hd(str, st_base);
		write(fd_out, str, ft_strlen(str));
		write(fd_out, "\n", 1);
		free(str);
		str = readline("> ");
	}
	return (0);
}

static void	set_status(int status, int *ex_stat)
{
	if (WIFEXITED(status))
		*ex_stat = WEXITSTATUS(status);
	else if (WIFEXITED(status) == 0 && WIFSIGNALED(status))
		*ex_stat = 1;
}

static int	make_heredoc(char *limiter, int *ex_stat, t_util *st_base)
{
	t_pid	pid;

	init_pid_sig_hd(&pid, st_base);
	pipe(pid.newpipe);
	pid.cu_pid = fork();
	if (pid.cu_pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(pid.newpipe[0]);
		read_heredoc(limiter, pid.newpipe[1], st_base);
		close(pid.newpipe[1]);
		exit(0);
	}
	else
	{
		close(pid.newpipe[1]);
		if (waitpid(pid.cu_pid, &pid.status, WUNTRACED | WCONTINUED) >= 0)
			set_status(pid.status, ex_stat);
	}
	if (*ex_stat == 1)
		return (reset_fd_sig_close(pid.newpipe[0], st_base));
	return (pid.newpipe[0]);
}

int	open_heredoc(t_pid *pid, int *ex_stat, t_util *st_base)
{
	int	i[2];

	while (pid->temp_cmd)
	{
		i[0] = 0;
		i[1] = 0;
		while (pid->temp_cmd->heredoc != NULL && pid->temp_cmd->heredoc[i[0]])
		{
			i[1] = make_heredoc(pid->temp_cmd->heredoc[i[0]], ex_stat, st_base);
			if (i[1] < 0)
				return (-1);
			if (pid->temp_cmd->fd_in == -3)
				pid->temp_cmd->fd_in = i[1];
			else if (pid->temp_cmd->fd_in > 2)
			{
				close(pid->temp_cmd->fd_in);
				pid->temp_cmd->fd_in = i[1];
			}				
			else
				close(i[1]);
			i[0] = i[0] + 1;
		}
		pid->temp_cmd = pid->temp_cmd->next;
	}
	return (0);
}
