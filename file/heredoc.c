#include "../minishell.h"

int	read_heredoc(char *limiter, int fd_out)
{
	char	*str;

	str = readline("> ");
	while (str)
	{
		if ((ft_strncmp(str, limiter, ft_strlen(limiter)) == 0) && ft_strlen(limiter) == ft_strlen(str))
					break ;
		str = check_expand_hd(str);
		write(fd_out, str, ft_strlen(str));
		write(fd_out, "\n", 1);
		free(str);
		str = readline("> ");
	}
	return (0);
}

int	make_heredoc(char *limiter)
{
	int	pid;
	int	newpipe[2];
	int	status;
	int	rc;

	signal(SIGINT, handle_sigint_hd);
	// signal(SIGQUIT, SIG_IGN);
	gl.termios_new.c_lflag &= ~(ECHOCTL);
	rc = tcsetattr(0, 0, &gl.termios_new);
	if (rc)
		exit(errtx_all_free_exit(1, "set termios failed\n"));
	pipe(newpipe);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(newpipe[0]);
		read_heredoc(limiter, newpipe[1]);
		close(newpipe[1]);
		exit(err_all_free_exit(0));
	}
	else
	{
		close(newpipe[1]);
		if (waitpid(pid, &status, WUNTRACED | WCONTINUED) >= 0)
		{
			if (WIFEXITED(status))
				gl.g_status = WEXITSTATUS(status);
			else if (WIFEXITED(status) == 0 && WIFSIGNALED(status))
				gl.g_status = 1;
		}
	}
	gl.termios_new.c_lflag |= (ECHOCTL);
	rc = tcsetattr(0, 0, &gl.termios_new);
	if (rc)
		exit(errtx_all_free_exit(1, "set termios failed\n"));
	if (gl.g_status == 1)
	{
		reset_fd_sig();
		close(newpipe[0]);
		return(-1);
	}	
	return (newpipe[0]);
}


