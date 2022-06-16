#include "../minishell.h"

int	check_file_access(t_cmd	*current)
{
	char	*expanded;
	char	*temp;

	int	i;
	int	j;
	expanded = NULL;
	temp = NULL;
	if (current->files == NULL)
		return (0);
	i = 0;
	j = 0;
	while (current->files[i])
	{
		expanded = check_expand_file(&current->files[i][1]);
		if (expanded == NULL)
		{
			gl.g_status = 1;
			return (return_errtx(-4, "ambiguous redirect\n"));
		}
		if (current->files[i][0] == '1')
		{
			if (access(expanded, F_OK) != 0
				|| (access(expanded, F_OK) == 0
					&& access(expanded, R_OK) < 0))
			{
				free(expanded);
				gl.g_status = 1;
				return (return_perr(-4, expanded));
			}
		}
		if (current->files[i][0] == '2' || current->files[i][0] == '3')
		{
			if (access(expanded, F_OK) == 0 && access(expanded, W_OK) < 0)
			{
				free(expanded);
				gl.g_status = 1;
				return (return_perr(-4, expanded));
			}
			if (access(expanded, F_OK) != 0)
			{
				j = open(expanded, O_CREAT, 0666);
				close(j);
			}
		}
		temp = malloc(sizeof(char) * ft_strlen(expanded) + 2);
		if (temp == NULL)
			errtx_all_free_exit(1, "malloc for temp");
		temp[0] = current->files[i][0];
		ft_strlcpy(&temp[1], expanded, ft_strlen(expanded) + 1);
		free(expanded);
		free(current->files[i]);
		current->files[i] = temp;
		i++;
	}
	return (0);
}

char	*ft_find_env_pathnum(char *envp[])
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = &envp[i][5];
			return (path);
		}
		i++;
	}
	return (NULL);
}

char	*ft_make_binpath(int i, char *path, char *cmd)
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
		bin[j] = path[j];
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


//	At this moment I need to know if the library path exists with F_OK,
//		it will check the access X_OK later.
char	*ft_findshell_path(char *cmd, char *envp[])
{
	int		i;
	char	*path;
	char	*bin;

	i = 0;
	path = ft_find_env_pathnum(envp);
	while (path != NULL && path[i] != '\0')
	{
		while (path[i] != ':' && path[i] != '\0')
			i++;
		if (path[i] == ':' || path[i] == '\0')
		{
			bin = ft_make_binpath(i, path, cmd);
			if (bin == NULL)
				return (NULL);
			if (access(bin, F_OK) == 0) 	
			{								
				//printf(CYN"loop: bin: [%s]\n"RES, bin);
				return (bin);
			}
			free(bin);
			if (path[i] == '\0')
				return (NULL);
			path = &path[i + 1];
			i = 0;
		}
	}
	//printf(CYN"Return bin: [%s]\n"RES, bin);
	return (NULL);
}

int	exec_no_pipe(void)
{
	pid_t	pid;

	if (check_file_access(gl.start_cmd) != 0)
		return (gl.g_status);
	
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
			ms_execve(gl.start_cmd);
		if (gl.start_cmd->fd_in > 0)
			close(gl.start_cmd->fd_in);
		if (gl.start_cmd->fd_out > 1)
			close(gl.start_cmd->fd_out);
		waitpid(pid, &gl.g_status, WUNTRACED | WCONTINUED);
		if (WIFEXITED(gl.g_status))
			gl.g_status = WEXITSTATUS(gl.g_status);
		else if (WIFEXITED(gl.g_status) == 0 && WIFSIGNALED(gl.g_status))
			gl.g_status = 128 + WTERMSIG(gl.g_status);
		// else
		// 	gl.g_status = 0;
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

			if (check_file_access(current) != 0)
			{
				gl.g_status = 1;
				exit(err_all_free_exit(1));
			}
			
			if (connect_fd(current) != 0)
			{
				gl.g_status = 1;
				exit(err_all_free_exit(1));
			}
			if (current->args == NULL)
				break;
			current->args = expand_array(current->args);

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

int	check_close(int i)
{
	if (i < 0)
		exit(err_all_free_exit(1));
	return (i);
}




int	run_cmd(void)
{
	t_cmd	*current;
	t_pid	pid;

	if (open_heredoc() != 0)
		return (gl.g_status);
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
