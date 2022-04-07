#include "minishell.h"


t_cmd	*make_dammy1(t_infos *info, t_cmd *strdammy)
{
	strdammy = malloc(sizeof(t_cmd) * 1);
	if (strdammy == NULL)
		errtext_exit("making dammy failed\n");
	strdammy->start_env = info->start_env;
	strdammy->args = ft_split("cat ", ' ');
	strdammy->fd_in = -2;
	strdammy->fd_out = 1;
	strdammy->infile = make_malloc_str("infile");
	strdammy->outfile = make_malloc_str(NULL);
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
	strdammy->args = ft_split("wc -l", ' ');
	strdammy->fd_in = 0;
	strdammy->fd_out = 1;
	strdammy->infile = make_malloc_str(NULL);
	strdammy->outfile = make_malloc_str(NULL);
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
		if (access(str->infile, F_OK) != 0)
			errtext_exit("infile not exist\n");
		if (access(str->infile, F_OK) == 0 && access(str->infile, R_OK) < 0)
			errtext_exit("can't access infile\n");
		str->fd_in = open(str->infile, O_RDONLY);	
		if (str->fd_in < 0)
			errtext_exit("file open failed\n");
	}
	if (str->fd_in == -3)
	{
		printf("here doc function!\n");
	}
	if (str->fd_out == -2)
	{
		if (access(str->outfile, F_OK) == 0 && access(str->outfile, W_OK) < 0)
			errtext_exit("outfile exist but not accessible\n");
		str->fd_out = open(str->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (str->fd_out < 0)
			errtext_exit("output file open failed\n");
	}
	if (str->fd_out == -3)
	{
		if (access(str->outfile, F_OK) == 0 && access(str->outfile, W_OK) < 0)
			errtext_exit("outfile exist but not accessible\n");
		str->fd_out = open(str->outfile, O_WRONLY | O_CREAT, 0666);
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

	current = str;
	envs = get_env_array(info->start_env);
	str = manage_in_out(str);
	if (current->next == NULL) //if no pipe
	{
		//check if that is the builtin
		if (current->fd_in != 0)
		{
			old_in = dup(0);
			dup2(current->fd_in, 0);
			close(current->fd_in);
		}
		if (current->fd_out != 1)
		{
			// dup2(current->fd_out, 1);
			// close(current->fd_out);
		}
		pid = fork();
		if (pid == 0)
		{
			path = ft_findshell_pass(str->args[0], envs);
			printf("path is %s\n", path);
			exit(0);
			execve(path, str->args, envs);
		}
		wait(0);
		dup2(old_in, 0);

	}
	return(0);
}

int	main()
{
	int	pid;
	int	pipe1[2];
	int	std_in;


	std_in = dup(0);
	pipe(pipe1);
	pid = fork();
	if (pid == 0)
	{
		dup2(pipe1[1], 1);
		close(pipe1[0]);
		close(pipe1[1]);
		write(pipe1[1], "Hi\n", 3);
		// ft_file_heredoc("hello", std_in, 1);
	}
	else
	{
		dup2(pipe1[0], 0);
		close(pipe1[0]);
		close(pipe1[1]);
		sleep(5);
		wait(0);
		printf("now parents");

	}
	return (0);
}

// int	main(int argc, char *argv[], char *envp[])
// {
// 	t_infos	info;
// 	char	*line;
// 	t_cmd	*str1 = NULL;
// 	t_cmd	*str2 = NULL;
// 	t_cmd	*str_start = NULL;


// 	(void) argc;
// 	(void) argv;
// 	ms_init(&info, envp);
// 	str1 = make_dammy1(&info, str1);
// 	str2 = make_dammy2(&info, str2);
// 	str1->next = NULL;
// 	str2->prev = str1;
// 	str_start = str1;
// 	line = readline(info.prompt);
// 	while (line)
// 	{
// 		line = check_expand(&info, line);
// 		if (ft_strlen(line) > 0)
// 		{
// 			printf("%s\n", line);
// 			add_history(line);
// 		}
// 		run_cmd(&info, str1);
// 		printf("command finished\n");
// 		// here parsing and make a linkedlist of t_cmd
// 		// after making t_cmd list, fork and execute
// 		// if (run_cmd(&info, str_start) < 0)
// 		// 	errtext_exit("executing failed\n");
// 		free(line);
// 		line = readline(info.prompt);
// 		printf("readline again %s\n", line);
// 	}
// 	printf("exit!\n");
// 	rl_clear_history();
// 	free_envlist(&info);
// 	free(line);
// 	return (0);
// }
