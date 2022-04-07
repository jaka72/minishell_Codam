#include "../minishell.h"

char	*ft_addtext_free(char *s1, char *s2, int *num)
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

char	*ft_checklimit(char *buff, char *limiter)
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
			errtext_exit("read for stdin failed\n");;
		if (buff_temp[0] == limiter[i])
			limitcheck = ft_addtext_free(limitcheck, buff_temp, &i);
		else if (buff_temp[0] == '\n' && i == (int)ft_strlen(limiter))
		{
			free(limitcheck);
			return (NULL);
		}
			
		else
			break ;
	}
	buff[0] = buff_temp[0];
	return (limitcheck);
}

char	*ft_write_free(int fd, char *checklimit)
{
	write(fd, checklimit, ft_strlen(checklimit));
	free(checklimit);
	return (NULL);
}

int	ft_file_heredoc(char *limiter, int fd_out)
{
	char	*checklimit;
	char	buff_last[2];
	int		rd;

	buff_last[1] = '\n';
	checklimit = NULL;
	rd = 1;
	write(1, "start\n", 6);
	while (rd == 1)
	{
		rd = read(0, buff_last, 1);
		// printf("%d ", rd);
		if (rd < 0)
			errtext_exit("read heredoc failed\n");
		if (buff_last[1] == '\n' && buff_last[0] == limiter[0])
		{
			checklimit = ft_checklimit(buff_last, limiter);
			if (checklimit == NULL)
				break ;
			checklimit = ft_write_free(fd_out, checklimit);
		}
		// write(fd_out, buff_last, 1);
		buff_last[1] = buff_last[0];
	}
	printf("heredoc end\n");
	return (0);
}
