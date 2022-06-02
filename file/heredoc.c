#include "../minishell.h"

char	*addtext_free(char *s1, char *s2, int *num)
{
	char	*str;
	int		i;
	int		j;

	j = 0;
	i = ft_strlen(s1);
	str = malloc(sizeof(char) * (i + 2));
	if (!str)
		errtext_exit("malloc for making delimiter\n");
	while (j < i)
	{
		str[j] = s1[j];
		j++;
	}
	str[j] = s2[0];
	j++;
	str[j] = '\0';
	*num = *num + 1;
	free(s1);
	return (str);
}

char	*write_free(int fd, char *checklimit)
{
	write(fd, checklimit, ft_strlen(checklimit));
	return (free_return_null(checklimit));
}

int	get_heredoc(char *limiter, int fd_out, t_infos *info)
{
	char	buff[1];
	int		rd;
	char	*exp;

	rd = 1;
	exp = malloc(sizeof(char) * 1);
	if (exp == NULL)
		errtext_exit("malloc failed\n");
	exp[0] = '\0';
	write(info->ini_fd[1], "> ", 2);
	while (rd == 1)
	{
		rd = read(info->ini_fd[0], buff, 1);
		if (rd < 0)
			errtext_exit("read heredoc failed\n");
		if (rd > 0 && buff[0] != '\n' && rd > 0 && buff[0] != ' ')
			exp = ft_add_c_free(exp, buff[0]);
		else if (rd > 0 && buff[0] == '\n')
		{
			if (exp[0] != '\0')
			{
				if (ft_strncmp(exp, limiter, ft_strlen(exp)) == 0)
					break ;
				exp = check_expand_hd(info, exp);
				exp = ft_add_c_free(exp, buff[0]);
				write(fd_out, exp, ft_strlen(exp));
				free(exp);
				exp = malloc(sizeof(char) * 1);
				if (exp == NULL)
					errtext_exit("malloc failed\n");
				exp[0] = '\0';
			}
			else
				write(fd_out, "\n", 1);
			write(info->ini_fd[1], "> ", 2);
		}
		else if (rd > 0 && buff[0] == ' ')
		{
			exp = check_expand_hd(info, exp);
			exp = ft_add_c_free(exp, buff[0]);
			write(fd_out, exp, ft_strlen(exp));
			free(exp);
			exp = malloc(sizeof(char) * 1);
			if (exp == NULL)
				errtext_exit("malloc failed\n");
			exp[0] = '\0';
		}
	}
	free(exp);
	return (0);
}

int	make_heredoc(char *limiter, t_infos *info)
{
	int	pid;
	int	newpipe[2];

	signal(SIGQUIT, handle_sigquit_hd);
	pipe(newpipe);
	pid = fork();
	if (pid == 0)
	{
		dup2(newpipe[1], 1);
		close(newpipe[0]);
		get_heredoc(limiter, newpipe[1], info);
		close(newpipe[1]);
		exit(1);
	}
	else
	{
		close(newpipe[1]);
		wait(0);
	}
	// write(2, "send!\n", 6);
	return (newpipe[0]);
}

// check! if "<< here" and no argument, it causes segmentation fault
