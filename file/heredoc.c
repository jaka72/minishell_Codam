#include "../minishell.h"

int	get_heredoc(char *limiter, int fd_out)
{
	char	buff[1];
	int		rd;
	char	*exp;

	rd = 1;
	exp = malloc(sizeof(char) * 1);
	if (exp == NULL)
		exit(err_all_free_exit(1));
	exp[0] = '\0';
	buff[0] = '\n';
	write(gl.ini_fd[1], "> ", 2);
	while (rd == 1 || (rd == 0 && buff[0] != '\n'))
	{
		rd = read(gl.ini_fd[0], buff, 1);
		if (rd < 0)
			exit(err_all_free_exit(1));
		if (rd > 0 && buff[0] != '\n' && rd > 0 && buff[0] != ' ')
			exp = ft_add_c_free(exp, buff[0]);
		else if (rd > 0 && buff[0] == '\n')
		{
			if (exp[0] != '\0')
			{
				if (ft_strncmp(exp, limiter, ft_strlen(limiter)) == 0)
					break ;
				exp = check_expand_hd(exp);
				exp = ft_add_c_free(exp, buff[0]);
				write(fd_out, exp, ft_strlen(exp));
				free(exp);
				exp = malloc(sizeof(char) * 1);
				if (exp == NULL)
					exit(err_all_free_exit(1));
				exp[0] = '\0';
			}
			else
			{
				write(fd_out, "\n", 1);
				write(gl.ini_fd[1], "\n", 1);
			}
				
			write(gl.ini_fd[1], "> ", 2);
		}
		else if (rd > 0 && buff[0] == ' ')
		{
			exp = check_expand_hd(exp);
			exp = ft_add_c_free(exp, buff[0]);
			write(fd_out, exp, ft_strlen(exp));
			free(exp);
			exp = malloc(sizeof(char) * 1);
			if (exp == NULL)
				exit(err_all_free_exit(1));
			exp[0] = '\0';
		}
	}
	free(exp);
	return (0);
}

int	make_heredoc(char *limiter)
{
	int	pid;
	int	newpipe[2];
	int	status;

	signal(SIGQUIT, handle_sigquit_hd);
	signal(SIGINT, handle_sigint_hd);
	pipe(newpipe);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		dup2(newpipe[1], 1);
		close(newpipe[0]);
		get_heredoc(limiter, newpipe[1]);
		close(newpipe[1]);
		exit(err_all_free_exit(0));
	}
	else
	{
		close(newpipe[1]);
		// wait(0);
		if (waitpid(pid, &status, WUNTRACED | WCONTINUED) >= 0)
		{
			if (WIFEXITED(status))
				gl.g_status = WEXITSTATUS(status);
			else if (WIFEXITED(status) == 0 && WIFSIGNALED(status))
				gl.g_status = 1;
		}
	}
	if (gl.g_status == 1)
	{
		reset_fd_sig();
		close(newpipe[0]);
		return(-1);
	}	
	return (newpipe[0]);
}

// check! if "<< here" and no argument, it causes segmentation fault
