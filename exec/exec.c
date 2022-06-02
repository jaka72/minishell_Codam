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

int	ms_execve(t_infos *info, t_cmd *str)
{
	char	**envs;
	char	*path;

	envs = get_env_array(info->start_env);
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
		exit(127);
	}
	execve(path, str->args, envs);
	exit(127);
}

int exec_no_pipe(t_infos *info, t_cmd *current, t_cmd *str)
{
	pid_t	pid;
	int		status;

	current->args = expand_array(current->args, info);
	connect_hd(current, info);
	connect_fd(current, info);
	if (check_if_builtin(current) == 1)
		g_status = exec_builtin(current, info, str);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			ms_execve(info, current);	
		}
		waitpid(pid, &status, WUNTRACED | WCONTINUED);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		if (WIFEXITED(status) == 0 && WIFSIGNALED(status))
			g_status = 128 + WTERMSIG(status);
	}
	return (0);
}

int	run_cmd(t_infos *info, t_cmd *str)
{
	t_cmd	*current;
	pid_t	pid;
	int		newpipe[2];
	int		oldpipe;
	int		status;
	pid_t	last_pid;

	current = str;
	status = 0;
	oldpipe = 0;
	signal(SIGINT, handle_sigint_p);
	signal(SIGQUIT, handle_sigquit_p);
	if (current->next == NULL)
		exec_no_pipe(info, current, str);
	else
	{
		while (current)
		{
			if (current->fd_in == -3)
				signal(SIGQUIT, handle_sigquit);
			else
				signal(SIGQUIT, handle_sigquit_p);
			if (current != str)
				g_status = 0;
			current->args = expand_array(current->args, info);
			if (current->next)
				pipe(newpipe);
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				if (current->fd_in == -3)
					signal(SIGQUIT, handle_sigquit);
				else
					signal(SIGQUIT, SIG_DFL);
				if (oldpipe != 0)
				{
					dup2(oldpipe, 0);
					close(oldpipe);
				}

				connect_hd(current, info);
				connect_fd(current, info);
				dup2(newpipe[1], 1);
				close(newpipe[1]);
				close(newpipe[0]);
				if (check_if_builtin(current) == 1)
					g_status = exec_builtin(current, info, str);
				else
					ms_execve(info, current);
				exit (0);  // in case of builtin, it should be quit
			}
			else
			{
				if (oldpipe != 0)
					close(oldpipe);
				oldpipe = newpipe[0];
				close(newpipe[1]);
				if (current->next == NULL)
					last_pid = pid;
				if (current->fd_in == -3)
				{
					
					if (waitpid(pid, &status, WUNTRACED | WCONTINUED) == last_pid)
					{
						// printf("last pid is %d\n", last_pid);
						if (WIFEXITED(status))
							g_status = WEXITSTATUS(status);
						if (WIFEXITED(status) == 0 && WIFSIGNALED(status))
							g_status = 128 + WTERMSIG(status);
					}
					if (WIFEXITED(status) == 0 && WIFSIGNALED(status))
					{							
						write(2, "quit\n", 5);
						break;
					}
				}		
			}
			current = current->next;
		}
		// waitpid(last_pid, &status, WUNTRACED | WCONTINUED);
		// if (WIFEXITED(status))
		// 	g_status = WEXITSTATUS(status);
		// if (WIFEXITED(status) == 0 && WIFSIGNALED(status))
		// 	g_status = 128 + WTERMSIG(status);
		current = str;
		while (current)
		{
			if (current->fd_in != -3)
			{
				if (waitpid(0, &status, WUNTRACED | WCONTINUED)  == last_pid)
				{
					// printf("last pid is %d\n", last_pid);
					if (WIFEXITED(status))
						g_status = WEXITSTATUS(status);
					if (WIFEXITED(status) == 0 && WIFSIGNALED(status))
						g_status = 128 + WTERMSIG(status);	
				}
			}
			current = current->next;
		}
	}
	reset_fd(info);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	// printf("g_status is %d\n", g_status);
	return(g_status);
}