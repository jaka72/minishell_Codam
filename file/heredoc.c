#include "../minishell.h"

int	read_heredoc(char *limiter, int fd_out)
{
	char	*str;

	str = readline("> ");
	while (str)
	{
		if ((ft_strncmp(str, limiter, ft_strlen(limiter)) == 0)
			&& ft_strlen(limiter) == ft_strlen(str))
			break ;
		str = check_expand_hd(str);
		write(fd_out, str, ft_strlen(str));
		write(fd_out, "\n", 1);
		free(str);
		str = readline("> ");
	}
	return (0);
}

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

int	make_heredoc(char *limiter)
{
	int	rc;
	t_pid	pid;

	init_pid_sig_hd(&pid);
	g_gl.termios_new.c_lflag &= ~(ECHOCTL);
	rc = tcsetattr(0, 0, &g_gl.termios_new);
	if (rc)
		exit(errtx_all_free_exit(1, "set termios failed\n"));
	pipe(pid.newpipe);
	pid.cu_pid = fork();
	if (pid.cu_pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(pid.newpipe[0]);
		read_heredoc(limiter, pid.newpipe[1]);
		close(pid.newpipe[1]);
		exit(err_all_free_exit(0));
	}
	else
	{
		close(pid.newpipe[1]);
		if (waitpid(pid.cu_pid, &pid.status, WUNTRACED | WCONTINUED) >= 0)
		{
			if (WIFEXITED(pid.status))
				g_gl.g_status = WEXITSTATUS(pid.status);
			else if (WIFEXITED(pid.status) == 0 && WIFSIGNALED(pid.status))
				g_gl.g_status = 1;
		}
	}
	g_gl.termios_new.c_lflag |= (ECHOCTL);
	rc = tcsetattr(0, 0, &g_gl.termios_new);
	if (rc)
		exit(errtx_all_free_exit(1, "set termios failed\n"));
	if (g_gl.g_status == 1)
	{
		reset_fd_sig();
		close(pid.newpipe[0]);
		return (-1);
	}	
	return (pid.newpipe[0]);
}

int	open_heredoc(t_pid *pid)
{
	int	i[2];

	while (pid->temp_cmd)
	{
		i[0] = 0;
		i[1] = 0;
		while (pid->temp_cmd->heredoc != NULL && pid->temp_cmd->heredoc[i[0]])
		{
			i[1] = make_heredoc(pid->temp_cmd->heredoc[i[0]]);
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
