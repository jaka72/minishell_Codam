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

char	*check_limiter(char *buff, char *limiter)
{
	char	buff_temp[1];
	char	*limitcheck;
	int		i;
	int		rd;

	i = 1;
	rd = 1;
	limitcheck = malloc(2);
	if (limitcheck == NULL)
		errtext_exit("malloc for limit check failed\n");
	ft_strlcpy(limitcheck, buff, 2);
	while (rd > 0 && i <= (int)ft_strlen(limiter))
	{
		rd = read(0, buff_temp, 1);
		if (rd < 0)
			errtext_exit("read for stdin failed\n");
		if (buff_temp[0] == limiter[i])
			limitcheck = addtext_free(limitcheck, buff_temp, &i);
		else if (buff_temp[0] == '\n' && i == (int)ft_strlen(limiter))
			return (free_return_null(limitcheck));
		else
			break ;
	}
	buff[0] = buff_temp[0];
	return (limitcheck);
}

char	*write_free(int fd, char *checklimit)
{
	write(fd, checklimit, ft_strlen(checklimit));
	return (free_return_null(checklimit));
}

int	get_heredoc(char *limiter, int fd_out)
{
	char	*checklimit;
	char	buff_last[2];
	int		rd;

	buff_last[1] = '\n';
	checklimit = NULL;
	rd = 1;
	while (rd == 1)
	{
		rd = read(STDIN_FILENO, buff_last, 1);
		if (rd < 0)
			errtext_exit("read heredoc failed\n");
		if (buff_last[1] == '\n' && buff_last[0] == limiter[0])
		{
			checklimit = check_limiter(buff_last, limiter);
			if (checklimit == NULL)
				break ;
			checklimit = write_free(fd_out, checklimit);
		}
		write(fd_out, buff_last, 1);
		buff_last[1] = buff_last[0];
	}
	return (0);
}

int	make_heredoc(char *limiter)
{
	int	pid;
	int	newpipe[2];

	pipe(newpipe);
	pid = fork();
	if (pid == 0)
	{
		dup2(newpipe[1], 1);
		close(newpipe[0]);
		get_heredoc(limiter, newpipe[1]);
		close(newpipe[1]);
		exit(1);
	}
	else
	{
		close(newpipe[1]);
		wait(0);
	}
	return (newpipe[0]);
}
