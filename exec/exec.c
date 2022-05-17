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
int	ms_execve(t_infos *info, t_cmd *str) // added jaka: if exit, return 2
{										 //				if other builtin, return 1
	char	**envs;						 // 			if normal cmd, run execve
	char	*path;
	int		ret;	// added jaka

	envs = get_env_array(info->start_env);
	str->args = expand_array(str->args, info);
	path = ft_findshell_pass(str->args[0], envs);

	// write(2, "ms_execve: ", 11);	// added jaka
	// write (2, path, 10);	// added jaka: path is NULL when command is 'exit', 'export', 'unset',
	// write (2, "\n", 1);		// added jaka

	ret = check_if_builtin(str, info); // added jaka: returns 0, 1 or 2
	printf(MAG"   RET is %d\n"RES, ret);
	if (path == NULL)	// jaka: path is NULL when command is 'exit', 'export', 'unset'
	{
		printf(MAG"   PATH is NULL\n"RES);
		if (ret == 1) // added jaka: other builtin found (not exit)
		{
			printf(MAG"      Other builtin is found (not exit)\n"RES);
			return (1); // added jaka
		}
		else if (ret == 2) // added jaka: EXIT builtin found
		{
			printf(MAG"      Builtin EXIT is found\n"RES);
			return (2); // added jaka
		}
		write(3, "command not found\n", 19);
		exit(127);
	}		
	else
	{	// added jaka
		if (ret == 0) // added jaka: Command is not a builtin
		{
			printf(MAG"    Cmd is NOT a builtin\n"RES);
			execve(path, str->args, envs);
		}
	}
	printf(MAG"    Cmd IS a builtin\n"RES);
	return (0);
}

int	run_cmd(t_infos *info, t_cmd *str)
{
	t_cmd	*current;
	pid_t	pid;
	int		newpipe[2];
	int		status;

	current = str; // THIS IS THE WHOLE CMD LIST, IT CAN BE USED IN EXIT TO COUNT HOW MANY COMMANDS
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
			printf(RED"run_cmd: start new child\n"RES);
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
			//if (check_if_builtin(str, info) == 0) 	// removed jaka
			if (ms_execve(info, current) == 2)			// changed jaka: found EXIT, must exit the program
			{
				printf(RED"run_cmd: ms_execve returned 2 (exit builtin)\n"RES);
				exit (222);								// added jaka: random value 222
			}
			printf(RED"run_cmd: execve was not called, because it is builtin\n"RES);
			
			exit (0);	// added jaka: if builtin was called, then this child process has to exit.
		}
		else
		{
			if (current->fd_out > 1)
				close(current->fd_out);
			if (current->fd_in > 0)
				close(current->fd_in);
		}
		current = current->next;
		//printf(YEL"jaka: after the child ended\n"RES);
	}

	waitpid(pid, &status, WUNTRACED | WCONTINUED);
	printf(BLU"child process exit status is %d\n"RES, status);
	if (WIFEXITED(status))
	{
		printf(BLU"child process ended with status %d\n"RES, WEXITSTATUS(status));
		if (WEXITSTATUS(status) == 222)	// added jaka
		{
			reset_fd(info);		// added jaka
			exit (0);			// added jaka
		}
	}
	if (WIFSIGNALED(status))
	{
		printf(BLU"child process ended with signal %d status %d\n"RES, WTERMSIG(status), WEXITSTATUS(status));
	}
	reset_fd(info);	
	return(0);
}
