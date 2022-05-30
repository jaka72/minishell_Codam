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

// void	ms_execve(t_infos *info, t_cmd *str)
int	ms_execve(t_infos *info, t_cmd *str) 
{
	char	**envs;
	char	*path;

	envs = get_env_array(info->start_env);
	if (envs == NULL)
		return (-1);
	path = ft_findshell_pass(str->args[0], envs);

	// printf(MAG"ms_execve(): command: str[0]: [%s] \n"RES, str->args[0]);
	if (path == NULL || str->args[0][0] == '\0')	// check the command is exist
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
		exit(127);
	}
	printf("first!\n");
	execve(path, str->args, envs);
	printf("hier!\n");
	// return (0)/;
	exit(127);
	}

int exec_no_pipe(t_infos *info, t_cmd *current)
{
	pid_t	pid;
	int		status;

	current->args = expand_array(current->args, info);
	connect_fd(current, info);
	if (check_if_builtin(current) == 1) 	// if builtin
	{
		//printf(BLU"no pipe and this is builtin!\n"RES);
		exec_builtin(current, info);
	}
	else // if library
	{
		// printf(BLU"no pipe and this is library!\n"RES);
		pid = fork();
		if (pid == 0)
		{
			// printf(RED"run_cmd(): current[0]: [%s]\n"RES, current->args[0]);
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			ms_execve(info, current);	
		}
		waitpid(pid, &status, WUNTRACED | WCONTINUED);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		if (WIFEXITED(status) == 0 && WIFSIGNALED(status))
			g_status = 128 + WTERMSIG(status);
		// printf(BLU"s child process exit status is %d\n"RES, status);
		// printf(BLU"s WIFEXITED(status) || WIFSIGNALED(status), WEXITSTATUS(status), WTERMSIG(status) %d %d %d %d\n"RES, WIFEXITED(status), WIFSIGNALED(status), WEXITSTATUS(status), WTERMSIG(status));
	}
	return (0);
}

int	run_cmd(t_infos *info, t_cmd *str)
{
	t_cmd	*current;
	pid_t	pid;
	int		newpipe[2];
	int		status;
	pid_t	last_pid;

	current = str;
	status = 0;
	if (current->next == NULL)
		exec_no_pipe(info, current);
	else  // with pipe
	{
		while (current)
		{
			if (current != str)
				g_status = 0;
			current->args = expand_array(current->args, info);
			if (current->next)
			{
				pipe(newpipe);
				if (current->next->fd_in == 0)
					current->next->fd_in = dup(newpipe[0]);
				if (current->fd_out == 1)
					current->fd_out = newpipe[1];
			}
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				connect_fd(current, info);
				// printf(RED"run_cmd(): current[0]: [%s]\n"RES, current->args[0]);
				if (check_if_builtin(current) == 1) 	// if builtin
					g_status = exec_builtin(current, info);
				else // if library
					ms_execve(info, current);
				exit (0);  // in case of builtin, it should be quit
			}
			else
			{
				if (current->next == NULL)
				{
					last_pid = pid;
					// printf(RED"last_pid is [%d]\n"RES, last_pid);
				}
					
				if (current->fd_out > 1)
					close(current->fd_out);
				if (current->fd_in > 0)
					close(current->fd_in);
			}

			current = current->next;
		}
		waitpid(last_pid, &status, WUNTRACED | WCONTINUED);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		if (WIFEXITED(status) == 0 && WIFSIGNALED(status))
			g_status = 128 + WTERMSIG(status);
		// printf(BLU"child process exit status is %d, g_status %d, last_pid is %d\n"RES, status, g_status, last_pid);
		// printf(BLU"WIFEXITED(status) || WIFSIGNALED(status), WEXITSTATUS(status), WSTOPSIG(status), SIGINT, SIGQUIT %d %d %d %d %d %d \n"RES, WIFEXITED(status), WIFSIGNALED(status), WEXITSTATUS(status), WSTOPSIG(status), SIGINT, SIGQUIT);
		current = str;
		while (current->next)
		{
			wait(0);
			current = current->next;
		}
		
	// while (wait(0))
	// 	;

	}
	reset_fd(info);
	// signal(SIGINT, handle_sigint);
	// signal(SIGQUIT, handle_sigquit);
	return(g_status);
}