#include "../minishell.h"

char	*ft_find_env_passnum(char *envp[])
{
	int		i;
	char	*pass;

	i = 0;
	pass = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			pass = &envp[i][5];
			return (pass);
		}
		i++;
	}
	return (NULL);
}

char	*ft_make_binpass(int i, char *pass, char *cmd)
{
	int		j;
	int		k;
	char	*bin;

	j = 0;
	k = 0;
	bin = malloc(i + 2 + ft_strlen(cmd));
	if (bin == NULL)
		return (NULL);
	while (j < i)
	{
		bin[j] = pass[j];
		j++;
	}
	bin[j] = '/';
	j++;
	while (k < (int)ft_strlen(cmd))
	{
		bin[j + k] = cmd[k];
		k++;
	}
	bin[j + k] = '\0';
	return (bin);
}

char	*ft_findshell_pass(char *cmd, char *envp[])
{
	int		i;
	char	*pass;
	char	*bin;

	i = 0;
	pass = ft_find_env_passnum(envp);
	while (pass != NULL && pass[i] != '\0')
	{
		while (pass[i] != ':' && pass[i] != '\0')
			i++;
		if (pass[i] == ':' || pass[i] == '\0')
		{
			bin = ft_make_binpass(i, pass, cmd);
			if (bin == NULL)
				return (NULL);
			if (access(bin, X_OK) == 0)
				return (bin);
			free(bin);
			if (pass[i] == '\0')
				return (NULL);
			pass = &pass[i + 1];
			i = 0;
		}
	}
	return (NULL);
}

int	ms_execve(t_cmd *str)
{
	char	**envs;
	char	*path;

	envs = get_env_array();
	if (envs == NULL)
		return (-1);
	path = ft_findshell_pass(str->args[0], envs);
	if (path == NULL || str->args[0][0] == '\0')
	{
		if (ft_strchr(str->args[0], '/') != NULL)
		{
			write(2, str->args[0], ft_strlen(str->args[0]));
			write(2, ": No such file or directory\n", 28);
		}
		else
		{
			write(2, str->args[0], ft_strlen(str->args[0]));
			write(2, ": command not found\n", 21);
		}
		exit(err_all_free_exit(127));
	}
	execve(path, str->args, envs);
	exit(err_all_free_exit(1));
}

int	exec_no_pipe(void)
{
	pid_t	pid;

	if (gl.start_cmd->args == NULL)
		return (gl.g_status);
	gl.start_cmd->args = expand_array(gl.start_cmd->args);
	if (connect_fd(gl.start_cmd) != 0)
		gl.g_status = 1;
	else if (check_if_builtin(gl.start_cmd) == 1)
		gl.g_status = exec_builtin(gl.start_cmd, gl.start_cmd);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			ms_execve(gl.start_cmd);
		}
		if (gl.start_cmd->fd_in > 0)
			close(gl.start_cmd->fd_in);
		if (gl.start_cmd->fd_out > 1)
			close(gl.start_cmd->fd_out);
		waitpid(pid, &gl.g_status, WUNTRACED | WCONTINUED);
		if (WIFEXITED(gl.g_status))
			gl.g_status = WEXITSTATUS(gl.g_status);
		else if (WIFEXITED(gl.g_status) == 0 && WIFSIGNALED(gl.g_status))
			gl.g_status = 128 + WTERMSIG(gl.g_status);
		else
			gl.g_status = 0;
	}
	reset_fd_sig();
	return (gl.g_status);
}

int	open_heredoc(void)
{
	t_cmd	*current;
	int	i;
	int	j;

	current = gl.start_cmd;
	while (current)
	{
		if (current->heredoc != NULL)
		{
			i = 0;
			j = 0;
			while (current->heredoc[i])
			{
				j = make_heredoc(current->heredoc[i]);
				if (j < 0)
					return (-1);
				if (current->fd_in == -3)
					current->fd_in = j;
				else if (current->fd_in > 2)
				{
					close(current->fd_in);
					current->fd_in = j;
				}				
				else
					close(j);
				i++;
			}
		}
		current = current->next;
	}
	return (0);
}


void	init_pid_sig(t_pid *pidinfo)
{
	pidinfo->status = 0;
	pidinfo->newpipe[0] = 0;
	pidinfo->newpipe[1] = 0;
	pidinfo->newpipe[2] = 0;
	signal(SIGINT, handle_sigint_p);
	signal(SIGQUIT, handle_sigquit_p);
}

int	exec_with_pipe(t_pid *pid)
{
	t_cmd	*current;

	current = gl.start_cmd;
	while (current)
	{
		if (current != gl.start_cmd)
			gl.g_status = 0;
		current->args = expand_array(current->args);
		if (current->next)
			pipe(pid->newpipe);
		pid->pid = fork();
		if (pid->pid == 0)
		{
			if (pid->newpipe[2] != 0)
			{
				dup2(pid->newpipe[2], 0);
				close(pid->newpipe[2]);
			}
			if (pid->newpipe[1] != 0)
			{
				dup2(pid->newpipe[1], 1);
				close(pid->newpipe[1]);
			}
			if (pid->newpipe[0] != 0)
				close(pid->newpipe[0]);
			if (connect_fd(current) != 0)
			{
				gl.g_status = 1;
				exit(err_all_free_exit(1));
			}
			if (check_if_builtin(current) == 1)
				gl.g_status = exec_builtin(current, gl.start_cmd);
			else
				ms_execve(current);
			exit(err_all_free_exit(gl.g_status));
		}
		else
		{
			if (pid->newpipe[2] != 0)
				close(pid->newpipe[2]);
			pid->newpipe[2] = pid->newpipe[0];
			if (pid->newpipe[1] != 0)
				close(pid->newpipe[1]);
			if (current->next == NULL)
				pid->last_pid = pid->pid;
		}
		current = current->next;
	}
	return (0);
}

int	open_outfile(void)
{
	t_cmd	*current;
	int	i;
	int	j;

	current = gl.start_cmd;
	while (current)
	{
		if (current->outfile != NULL)
		{
			i = 0;
			j = 0;
			while (current->outfile[i])
			{
				current->outfile[i] = check_expand(current->outfile[i]);
				if (current->outfile[i] && access(current->outfile[i], F_OK) != 0)
				{
					j = open(current->outfile[i], O_CREAT, 0666);
					if (j < 0)
						exit(err_all_free_exit(1));
					close(j);
				}
				i++;
			}
		}
		current = current->next;
	}
	return (0);
}

int	run_cmd(void)
{
	t_cmd	*current;
	t_pid	pid;

	if (open_heredoc() != 0)
		return (gl.g_status);
	open_outfile();
	init_pid_sig(&pid);
	if (gl.start_cmd->next == NULL)
		return (exec_no_pipe());
	exec_with_pipe(&pid);
	current = gl.start_cmd;
	while (current)
	{
		if (waitpid(0, &pid.status, WUNTRACED | WCONTINUED) == pid.last_pid)
		{
			if (WIFEXITED(pid.status))
				gl.g_status = WEXITSTATUS(pid.status);
			else if (WIFEXITED(pid.status) == 0 && WIFSIGNALED(pid.status))
				gl.g_status = 128 + WTERMSIG(pid.status);
		}
		current = current->next;
	}
	reset_fd_sig();
	return (gl.g_status);
}
