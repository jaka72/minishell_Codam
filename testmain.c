#include "minishell.h"

t_cmd	*make_dammy1(t_infos *info, t_cmd *strdammy)
{
	strdammy = malloc(sizeof(t_cmd) * 1);
	if (strdammy == NULL)
		errtext_exit("making dammy failed\n");
	strdammy->start_env = info->start_env;
	strdammy->args = ft_split("cat", ' ');
	strdammy->fd_in = 0;
	strdammy->fd_out = 1;
	strdammy->infile = ft_split("infile", ' ');
	strdammy->outfile = NULL;
	strdammy->next = NULL;
	strdammy->prev = NULL;
	return (strdammy);
}

t_cmd	*make_dammy2(t_infos *info, t_cmd *strdammy)
{
	strdammy = malloc(sizeof(t_cmd) * 1);
	if (strdammy == NULL)
		errtext_exit("making dammy failed\n");
	strdammy->start_env = info->start_env;
	strdammy->args = ft_split("cat", ' ');
	strdammy->fd_in = 0;
	strdammy->fd_out = 1;
	strdammy->infile = NULL;
	strdammy->outfile = NULL;
	strdammy->next = NULL;
	strdammy->prev = NULL;
	return (strdammy);
}

t_cmd	*make_dammy3(t_infos *info, t_cmd *strdammy)
{
	strdammy = malloc(sizeof(t_cmd) * 1);
	if (strdammy == NULL)
		errtext_exit("making dammy failed\n");
	strdammy->start_env = info->start_env;
	strdammy->args = ft_split("wc", ' ');
	strdammy->fd_in = 0;
	strdammy->fd_out = 1;
	strdammy->infile = NULL;
	strdammy->outfile = NULL;
	strdammy->next = NULL;
	strdammy->prev = NULL;
	return (strdammy);
}

t_cmd	*manage_in_out(t_cmd *str)
{
	int	i;

	i = 0;
	if (str->fd_in == -2)
	{
		while (str->infile[i])
		{
			if (str->infile[i] == NULL || access(str->infile[i], F_OK) != 0)
				errtext_exit("infile not exist\n");
			// printf("%s\n", str->infile[i]);
			i++;
		}
		i--;
		if (access(str->infile[i], F_OK) == 0 && access(str->infile[i], R_OK) < 0)
			errtext_exit("can't access infile\n");
		str->fd_in = open(str->infile[i], O_RDONLY);	
		if (str->fd_in < 0)
			errtext_exit("file open failed\n");
	}
	if (str->fd_in == -3)
	{
		printf("here doc function!\n");
	}
	i = 0;
	if (str->fd_out == -2)
	{
		while (str->outfile[i])
		{
			if (access(str->outfile[i], F_OK) == 0 && access(str->outfile[i], W_OK) < 0)
				errtext_exit("outfile exist but not accessible\n");
			i++;
		}
		str->fd_out = open(str->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (str->fd_out < 0)
			errtext_exit("output file open failed\n");
	}
	i = 0;
	if (str->fd_out == -3)
	{
		while (str->outfile[i])
		{
			if (access(str->outfile[i], F_OK) == 0 && access(str->outfile[i], W_OK) < 0)
				errtext_exit("outfile exist but not accessible\n");
			i++;
		}
		str->fd_out = open(str->outfile[i], O_WRONLY | O_CREAT, 0666);
		if (str->fd_out < 0)
			errtext_exit("output file open failed\n");
	}
	return (str);
}

int	run_cmd(t_infos *info, t_cmd *str)
{
	t_cmd	*current;
	pid_t	pid;
	char	**envs;
	char	*path;
	int		old_in = 0;
	int		old_out = 1;
	int		newpipe[2];

	current = str;
	envs = get_env_array(info->start_env);
	old_in = dup(0);
	old_out = dup(1);
	while (current)
	{
		if (current->next)
		{
			pipe(newpipe);
			if (current->next->fd_in == 0)
				current->next->fd_in = dup(newpipe[0]);
			dup2(newpipe[1], 1);
			close(newpipe[1]);
			close(newpipe[0]);
		}
		else
		{
			dup2(old_out, 1);
		}
		if (current->fd_out > 1)
		{
			dup2(current->fd_out, 1);
			close(current->fd_out);
		}

		current = manage_in_out(current);
		//check if that is the builtin
		pid = fork();
		if (pid == 0)
		{
			if (current->fd_in != 0)
			{
				dup2(current->fd_in, 0);
				close(current->fd_in);
			}
			path = ft_findshell_pass(current->args[0], envs);
			
			execve(path, current->args, envs);
		}
		else
		{
			if (current->fd_in != 0)
			{
				close(current->fd_in);
			}
		}
		wait(0);

		current = current->next;
	}
	dup2(old_in, 0);
	dup2(old_out, 1);
	close(old_in);
	close(old_out);
	printf("run cmd finished!\n");
	return(0);
}

// int	main() // main for heredoc test
// {
// 	int	next_read_fd;
// 	char	buf[1];

// 	next_read_fd = ft_heredoc("hello"); //give delimiter and it returns the fd to read
// 	while (read(next_read_fd, buf, 1))
// 	{
// 		write(1, "!", 1);
// 		write(1, buf, 1);
// 	}
// 	close(next_read_fd);
// 	return (0);
// }


int	main(int argc, char *argv[], char *envp[])
{
	t_infos	info;
	char	*line;
	t_cmd	*str1 = NULL;
	t_cmd	*str2 = NULL;
	t_cmd	*str3 = NULL;
	t_cmd	*str_start = NULL;


	(void) argc;
	(void) argv;
	ms_init(&info, envp);
	str1 = make_dammy1(&info, str1);
	str2 = make_dammy2(&info, str2);
	str3 = make_dammy3(&info, str3);
	str1->next = str2;
	str2->prev = str1;
	str2->next = str3;
	str3->prev = str2;
	str_start = str1;
	while ((line = readline(info.prompt)))
	{
		line = check_expand(&info, line);
		if (ft_strlen(line) > 0)
		{
			// printf("%s\n", line);
			add_history(line);
		}
		run_cmd(&info, str1);
		printf("\ncommand finished\n");
		// here parsing and make a linkedlist of t_cmd
		// after making t_cmd list, fork and execute
		// if (run_cmd(&info, str_start) < 0)
		// 	errtext_exit("executing failed\n");
		free(line);
	}
	printf("exit!\n");
	rl_clear_history();
	free_envlist(&info);
	free_tcmd(str1);
	free(line);
	tcsetattr(0, 0, &info.termios_save);
	return (0);
}
