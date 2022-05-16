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

void	ms_execve(t_infos *info, t_cmd *str)
{
	char	**envs;
	char	*path;

	envs = get_env_array(info->start_env);
	str->args = expand_array(str->args, info);
	path = ft_findshell_pass(str->args[0], envs);
	if (path == NULL)
	{
		write(3, "command not found\n", 19);
		exit(127);
	}		
	else
		execve(path, str->args, envs);
}

int	run_cmd(t_infos *info, t_cmd *str)
{
	t_cmd	*current;
	pid_t	pid;
	int		newpipe[2];
	int		status;

	current = str;
	status = 0;
	while (current)
	{
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
			current = manage_in_out(current, info);
			if (current->fd_in > 0)
			{
				dup2(current->fd_in, 0);
				close(current->fd_in);
			}
			if (current->fd_out > 1)
			{
				dup2(current->fd_out, 1);
				close(current->fd_out);
			}
			//check if that is the builtin
			// if(check_if_builtin(str, info) == 0)
			ms_execve(info, current);
		}
		else
		{
			if (current->fd_out > 1)
				close(current->fd_out);
			if (current->fd_in > 0)
				close(current->fd_in);
		}
		current = current->next;
	}
	waitpid(pid, &status, WUNTRACED | WCONTINUED);
	printf("child process exit status is %d\n", status);
	if (WIFEXITED(status)) {
        printf("child process ended with status %d\n", WEXITSTATUS(status));
      }
    if (WIFSIGNALED(status))
	{
		printf("child process ended with signal %d status %d\n", WTERMSIG(status),  WEXITSTATUS(status));
	}
	reset_fd(info);	
	return(0);
}
