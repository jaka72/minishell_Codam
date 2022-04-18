
# include "check_syntax/syntax_header.h"
# include "parsing/utils.h"
# include "minishell.h"

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
		j = open(str->infile[i], O_RDONLY);
		if (j < 0)
			errtext_exit("file open failed\n");
		if (str->fd_in == -2)
			str->fd_in = j;
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

int	main(int argc, char *argv[], char *envp[])
{
	t_source	src;		// jaka
	t_cmd		*cmd_list;	// jaka

	t_infos	info;
	char	*line;

	(void) argc;
	(void) argv;
	ms_init(&info, envp);


	src.inputline = NULL;
	if (argc == 2)
	{
		src.inputline = argv[1];
		//printf(CYN"line len: %ld\n"RES, src.inputline_size);
	}
	else
	{
		line = readline(info.prompt);
		src.inputline = line;		// jaka
		if (src.inputline == NULL || src.inputline[0] == '\0')
			return (0);
		while (line)
		{
			line = check_expand(&info, line);
			if (ft_strlen(line) > 0)
				add_history(line);
			// here parsing and make a linkedlist of t_cmd
			// after making t_cmd list, fork and execute


			if (check_syntax_errors(&src) != 0)
				//return (SYNTAX_ERROR);		// exit code changes from 258 to 2
				return (258);		// exit code changes from 258 to 2

			cmd_list = make_commands(&src);


			run_cmd(&info, cmd_list);
			free(line);
			line = readline(info.prompt);
			src.inputline = line;		// jaka
			if (src.inputline == NULL || src.inputline[0] == '\0')
				return (0);

		}
	}

	
	printf("exit!\n");
	rl_clear_history();
	free_envlist(&info);
	free_tcmd(str1);
	// free(line);
	tcsetattr(0, 0, &info.termios_save);
	return (0);
}
