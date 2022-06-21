#include "../minishell.h"
#include <fcntl.h>

static int	set_infile(t_cmd *str, int i)
{
	int	j;

	j = 0;
	if (str->files[i][0] == '1')
	{
		j = open(&str->files[i][1], O_RDONLY);
		if (j < 0)
			return (-4);
		if (str->fd_in > 2)
			close (str->fd_in);
		str->fd_in = j;
	}
	return (0);
}

static int	set_outfile(t_cmd *str, int i)
{
	int	j;

	j = 0;
	if (str->files[i][0] == '2')
	{
		j = open(&str->files[i][1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (j < 0)
			return (-4);
		if (str->fd_out > 2)
			close (str->fd_out);
		str->fd_out = j;
	}
	return (0);
}

static int	set_outfile_ap(t_cmd *str, int i)
{
	int	j;

	j = 0;
	if (str->files[i][0] == '3')
	{
		j = open(&str->files[i][1], O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (j < 0)
			return (-4);
		if (str->fd_out > 2)
			close (str->fd_out);
		str->fd_out = j;
	}
	return (0);
}

int	open_file_fd(t_cmd *str)
{
	int	i;

	i = 0;
	if (str->files == NULL)
		return (0);
	while (str->files[i])
	{
		if (str->fd_in == -2 || str->fd_in > 2)
		{
			if (set_infile(str, i) != 0)
				return (-4);
		}
		if (str->fd_out == -2 || str->fd_out == -3 || str->fd_out > 2)
		{
			if (set_outfile(str, i) != 0)
				return (-4);
			if (set_outfile_ap(str, i) != 0)
				return (-4);
		}
		i++;
	}
	return (0);
}
