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

// char	*check_limiter(char *buff, char *limiter)
// {
// 	char	buff_temp[2];
// 	char	*limitcheck;
// 	int		i;
// 	int		rd;

// 	i = 1;
// 	rd = 1;
// 	buff_temp[1] = '\0';
// 	limitcheck = malloc(2);
// 	if (limitcheck == NULL)
// 		errtext_exit("malloc for limit check failed\n");
// 	// limitcheck[1] = '\0';
// 	ft_strlcpy(limitcheck, buff, 2);
// 	while (rd > 0 && i <= (int)ft_strlen(limiter))
// 	{
// 		rd = read(0, buff_temp, 1);
// 		if (rd < 0)
// 			errtext_exit("read for stdin failed\n");
// 		if (buff_temp[0] == limiter[i])
// 			limitcheck = addtext_free(limitcheck, buff_temp, &i);
// 		else if (buff_temp[0] == '\n' && i == (int)ft_strlen(limiter))
// 			return (free_return_null(limitcheck));
// 		else
// 			break ;
// 	}
// 	buff[0] = buff_temp[0];
// 	return (limitcheck);
// }

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


// int	get_heredoc(char *limiter, int fd_out, t_infos *info)
// {
// 	char	*checklimit;
// 	char	buff_last[2];
// 	int		rd;
// 	char	*exp;
// 	int		i;
// 	int 	j;

// 	buff_last[1] = '\n';
// 	checklimit = NULL;
// 	rd = 1;
// 	exp = NULL;
// 	j = 0;
// 	write(info->ini_fd[1], "> ", 2);
// 	while (rd == 1)
// 	{
// 		rd = read(info->ini_fd[0], buff_last, 1);
// 		if (rd < 0)
// 			errtext_exit("read heredoc failed\n");
// 		if (rd > 0 && buff_last[0] == '\n')
// 			write(info->ini_fd[1], "> ", 2);
// 		if (buff_last[1] == '\n' && buff_last[0] == limiter[0])
// 		{
// 			checklimit = check_limiter(buff_last, limiter);
// 			if (checklimit == NULL)
// 				break ;
// 			// checklimit = check_expand(info, checklimit);
// 			if (ft_strchr(checklimit, '$') != NULL)
// 			{
// 				i = ft_strlen(ft_strchr(checklimit, '$'));
// 				exp = malloc(sizeof(char) * i + 1);
// 				if (exp == NULL)
// 					errtext_exit("malloc failed\n");
// 				ft_strlcpy(exp, ft_strchr(checklimit, '$'), i + 1);
// 				while (j < (int)(ft_strlen(checklimit) - i))
// 				{
// 					write(fd_out, &checklimit[j], 1);
// 					j++;
// 				}
// 				free(checklimit);
// 			}
// 			else
// 				checklimit = write_free(fd_out, checklimit);
// 		}
// 		// if $ shows up, check the env
// 		else if (buff_last[0] == '$')
// 		{
// 			exp = malloc(sizeof(char) * 2);
// 			if (exp == NULL)
// 				errtext_exit("malloc failed\n");
// 			exp[0] = '$';
// 			exp[1] = '\0';
// 		}
// 		else if (exp != NULL)
// 		{
// 			if (buff_last[0] == '\n' || buff_last[0] == ' ' || buff_last[0] == '\0')
// 			{
// 				exp = check_expand(info, exp);
// 				if (exp)
// 				{
// 					write(fd_out, exp, ft_strlen(exp));
// 					free(exp);
// 				}
// 				exp = NULL;
// 				if (buff_last[0] == '\n' || buff_last[0] == ' ')
// 					write(fd_out, buff_last, 1);
// 			}
// 			else
// 				exp = ft_add_c_free(exp, buff_last[0]);
// 		}
// 		else if (exp == NULL)
// 			write(fd_out, buff_last, 1);
// 		buff_last[1] = buff_last[0];
// 	}
// 	if (exp != NULL)
// 		free(exp);
// 	return (0);
// }


int	make_heredoc(char *limiter, t_infos *info)
{
	int	pid;
	int	newpipe[2];

	// signal(SIGINT, SIG_DFL);
	// signal(SIGQUIT, SIG_DFL);
	pipe(newpipe);
	pid = fork();
	if (pid == 0)
	{
		// signal(SIGINT, SIG_DFL);
		// signal(SIGQUIT, SIG_DFL);
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
