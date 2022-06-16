#include "../minishell.h"


int	open_file_fd(t_cmd *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;

	if (str->files == NULL)
		return (0);
	while (str->files[i])
	{
		if (str->fd_in == -2)
		{	

			if (str->files[i][0] == '1')
				j = open(&str->files[i][1], O_RDONLY);
			if (j < 0)
				return (-4);
			if (str->fd_in > 2)
				close (str->fd_in);
			str->fd_in = j;

		}
		if (str->fd_out == -2 || str->fd_out == -3)
		{
			if (str->files[i][0] == '2')
			{
				j = open(&str->files[i][1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
				if (j < 0)
					return (-4);
				if (str->fd_out > 2)
					close (str->fd_out);
				str->fd_out = j;

			}
				
			else if (str->files[i][0] == '3')
			{
				j = open(&str->files[i][1], O_WRONLY | O_CREAT | O_APPEND, 0666);
				if (j < 0)
					return (-4);
				if (str->fd_out > 2)
					close (str->fd_out);
				str->fd_out = j;

			}
		}
		i++;
	}
	return (0);
}

int	connect_fd(t_cmd *current)
{
	open_file_fd(current);
		
	if (current->fd_in > 0)
	{
		dup2(current->fd_in, 0);
		close(current->fd_in);
		current->fd_in = 0;
	}
	if (current->fd_out > 1)
	{
		dup2(current->fd_out, 1);
		close(current->fd_out);
		current->fd_out = 1;
	}
	return (0);
}

void	reset_fd_sig(void)
{
	int	fd0;
	int	fd1;

	fd0 = dup(gl.ini_fd[0]);
	fd1 = dup(gl.ini_fd[1]);
	dup2(fd0, 0);
	dup2(fd1, 1);
	close(fd0);
	close(fd1);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}
