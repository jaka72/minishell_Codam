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
	// str->args = expand_array(str->args, info);
	path = ft_findshell_pass(str->args[0], envs);

	printf(MAG"ms_execve(): command: str[0]: [%s] \n"RES, str->args[0]);

	if (path == NULL)	// check the command is exist
	{
		write(2, str->args[0], ft_strlen(str->args[0]));
		write(2, ": command not found\n", 21);
		exit(127);
	}
	execve(path, str->args, envs);
	return (0);
}

int	run_cmd(t_infos *info, t_cmd *str)
{
	t_cmd	*current;
	pid_t	pid;
	int		newpipe[2];
	int		status;

	current = str;
	status = 0;
	current->args = expand_array(current->args, info);
	if (current->next == NULL) // the case of no pipe
	{
		connect_fd(current, info);
		if (check_if_builtin(current) != 0) 	// if builtin
		{
			//printf(BLU"no pipe and this is builtin!\n"RES);
			exec_builtin(current, info);
		}
		else // if library
		{
			printf(BLU"no pipe and this is library!\n"RES);
			pid = fork();
			if (pid == 0)
			{
				printf(RED"run_cmd(): current[0]: [%s]\n"RES, current->args[0]);
				ms_execve(info, current);	
			}
			waitpid(pid, &status, WUNTRACED | WCONTINUED);
			printf(BLU"child process exit status is %d\n"RES, status);
			if (WIFEXITED(status))
			{
				g_status = WEXITSTATUS(status); 
				printf(BLU"child process ended with status %d\n"RES, WEXITSTATUS(status));
			}
			if (WIFSIGNALED(status))
			{
				g_status = WEXITSTATUS(status); 
				printf(BLU"child process ended with signal %d status %d\n"RES, WTERMSIG(status), WEXITSTATUS(status));
			}
			current = current->next;	
		}
	}
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
				connect_fd(current, info);
				printf(RED"run_cmd(): current[0]: [%s]\n"RES, current->args[0]);
				if (check_if_builtin(current) != 0) 	// if builtin
				{
					printf("this is builtin!\n");
					g_status = exec_builtin(current, info);
				}
				else // if library
				{
					printf("this is library!\n");
					ms_execve(info, current);	
				}
				exit (0);  // in case of builtin, it should be quit
			}
			else
			{
				if (current->fd_out > 1)
					close(current->fd_out);
				if (current->fd_in > 0)
					close(current->fd_in);
			}
			waitpid(pid, &status, WUNTRACED | WCONTINUED);
			printf(BLU"child process exit status is %d\n"RES, status);
			if (WIFEXITED(status))
			{
				g_status = WEXITSTATUS(status); 
				printf(BLU"child process ended with status %d\n"RES, WEXITSTATUS(status));
			}
			if (WIFSIGNALED(status))
			{
				g_status = WEXITSTATUS(status); 
				printf(BLU"child process ended with signal %d status %d\n"RES, WTERMSIG(status), WEXITSTATUS(status));
			}
			current = current->next;
			//printf(YEL"jaka: after the child ended\n"RES);
		}
	}
	reset_fd(info);	
	return(g_status);
}
