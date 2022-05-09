#include "minishell.h"

t_cmd	*make_dammy1(t_infos *info, t_cmd *strdammy)
{
	strdammy = malloc(sizeof(t_cmd) * 1);
	if (strdammy == NULL)
		errtext_exit("making dammy failed\n");
	strdammy->start_env = info->start_env;
	strdammy->args = ft_split("cat", ' ');
	strdammy->fd_in = -3;
	strdammy->fd_out = 1;
	strdammy->infile = 	ft_split("infile outfile", ' ');
	// strdammy->infile = 	NULL;
	strdammy->outfile = NULL;
	strdammy->heredoc = ft_split("here", ' ');
	strdammy->count_args = 0;
	strdammy->count_infiles = 0;
	strdammy->count_outfiles = 0;
	strdammy->count_heredocs = 0;
	strdammy->next = NULL;
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
	strdammy->fd_out = -2;
	strdammy->infile = NULL;
	strdammy->outfile = ft_split("out1 out2 out3", ' ');
	// strdammy->outfile = NULL;
	strdammy->heredoc = NULL;
	// strdammy->heredoc = ft_split("here heredoc", ' ');
	strdammy->count_args = 0;
	strdammy->count_infiles = 0;
	strdammy->count_outfiles = 0;
	strdammy->count_heredocs = 0;
	strdammy->next = NULL;
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
	strdammy->heredoc = NULL;
	strdammy->count_args = 0;
	strdammy->count_infiles = 0;
	strdammy->count_outfiles = 0;
	strdammy->count_heredocs = 0;
	strdammy->next = NULL;
	return (strdammy);
}

void	check_infile_fd(t_cmd *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str->infile != NULL)
	{
		while (str->infile[i])
		{
			if (access(str->infile[i], F_OK) != 0)
				errtext_exit("infile not exist\n");
			if (access(str->infile[i], F_OK) == 0 && access(str->infile[i], R_OK) < 0)
				errtext_exit("can't access infile\n");
			i++;
		}
		if (i > 0)
			i--;
		if (str->fd_in == -2)
		{
			j = open(str->infile[i], O_RDONLY);
			if (j < 0)
				errtext_exit("file open failed\n");
			str->fd_in = j;
		}
			
	}
}

void	check_heredoc_fd(t_cmd *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str->heredoc != NULL)
	{
		while (str->heredoc[i])
		{
			j = make_heredoc(str->heredoc[i]);
			i++;
		}	
		if (str->fd_in == -3)
			str->fd_in = j;
	}
}

void	check_outfile_fd(t_cmd *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str->outfile != NULL)
	{
		while (str->outfile[i])
		{
			if (access(str->outfile[i], F_OK) == 0
				&& access(str->outfile[i], W_OK) < 0)
				errtext_exit("outfile exist but not accessible\n");
			if (access(str->outfile[i], F_OK) != 0)
			{
				str->fd_out = open(str->outfile[i], O_CREAT, 0666);
				if (str->fd_out < 0)
					errtext_exit("couldn't make output file\n");
				close(str->fd_out);
			}
			i++;
		}
		i--;
		if (str->fd_out == -2)
			str->fd_out = open(str->outfile[i],
					O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (str->fd_out == -3)
			str->fd_out = open(str->outfile[i], O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (str->fd_out < 0)
			errtext_exit("output file open failed\n");
	}
}

t_cmd	*manage_in_out(t_cmd *str)
{
	check_infile_fd(str);
	check_heredoc_fd(str);
	check_outfile_fd(str);
	return (str);
}

void	reset_fd(t_infos *info)
{
	dup2(info->ini_fd[0], 0);
	dup2(info->ini_fd[1], 1);
	close(info->ini_fd[0]);
	close(info->ini_fd[1]);
}

void	ms_execve(t_infos *info, t_cmd *str)
{
	char	**envs;
	char	*path;

	envs = get_env_array(info->start_env);
	path = ft_findshell_pass(str->args[0], envs);		
	execve(path, str->args, envs);
}

int	run_cmd(t_infos *info, t_cmd *str)
{
	t_cmd	*current;
	pid_t	pid;
	int		newpipe[2];

	current = str;
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
			dup2(info->ini_fd[1], 1);
		//check if that is the builtin and without pipe
		pid = fork();
		if (pid == 0)
		{
			current = manage_in_out(current);
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
			ms_execve(info, current);
		}
		wait(0);
		current = current->next;
	}
	reset_fd(info);
	return(0);
}

// int	main(int argc, char *argv[], char *envp[])
// {
// 	t_infos	info;
// 	char	*line;
// 	t_cmd	*str1 = NULL;
// 	t_cmd	*str2 = NULL;
// 	t_cmd	*str3 = NULL;

// 	(void) argc;
// 	(void) argv;
// 	ms_init(&info, envp);
// 	str1 = make_dammy1(&info, str1);
// 	str2 = make_dammy2(&info, str2);
// 	str3 = make_dammy3(&info, str3);
// 	str1->next = str2;
// 	str2->next = NULL;
// 	line = readline(info.prompt);
// 	// while (line)
// 	// {
// 		// line = check_expand(&info, line);
// 		if (ft_strlen(line) > 0)
// 			add_history(line);
// 		// here parsing and make a linkedlist of t_cmd
// 		// after making t_cmd list, fork and execute
// 		run_cmd(&info, str1);
// 		free(line);
// 		// line = readline(info.prompt);
// 	// }
// 	printf("exit!\n");
// 	rl_clear_history();
// 	free_envlist(&info);
// 	free_tcmd(str1);
// 	// free(line);
// 	tcsetattr(0, 0, &info.termios_save);
// 	return (0);
// }

int	main(int argc, char *argv[], char *envp[])
{
	t_infos	info;
	char	*tx;
	t_env	*env;

	(void) argc;
	(void) argv;
	tx = NULL;
	ms_init(&info, envp);
	env = info.start_env;
	// while (env)
	// {
	// 	printf("%s : %s \n", env->name, env->value);
	// 	env = env->next;
	// }
	printf("\'hello$PWD\' : %s\n", tx = check_expand(&info, "\'hello$PWD\'"));
	free (tx);
	printf("\"hello$PWD\" : %s\n", tx = check_expand(&info, "\"hello$PWD\""));
	free (tx);
	printf("hello$PWD : %s\n", tx = check_expand(&info, "hello$PWD"));
	free (tx);
	printf("hello $PWD : %s\n", tx = check_expand(&info, "hello $PWD"));
	free (tx);
	printf("$5 : %s\n", tx = check_expand(&info, "$5"));
	free (tx);
	printf("$a : %s\n", tx = check_expand(&info, "$a"));
	free (tx);
	printf("$pp : %s\n", tx = check_expand(&info, "$pp"));
	free (tx);
	printf("hello$PWD123 : %s\n", tx = check_expand(&info, "hello$PWD123"));
	free (tx);
	printf("\"hello$PWD $PWD\" : %s\n", tx = check_expand(&info, "\"hello$PWD $PWD\""));
	free (tx);
	free_envlist(&info);
	return (0);
}
