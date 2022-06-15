#include "../minishell.h"

int	check_infile_avairable(t_cmd *str)
{
	int	i;

	i = 0;
	if (str->infile == NULL)
		return (0);
	while (str->infile[i])
	{
		str->infile[i] = check_expand(str->infile[i]);
		if (str->infile[i] == NULL)
			return (return_errtx(-4, "ambiguous redirect\n"));
		if (access(str->infile[i], F_OK) != 0
			|| (access(str->infile[i], F_OK) == 0
				&& access(str->infile[i], R_OK) < 0))
		{
			gl.g_status = 1;
			return (return_perr(-4, str->infile[i]));
		}		
		i++;
	}
	return (0);
}

int	check_infile_fd(t_cmd *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str->infile != NULL)
	{
		while (str->infile[i])
		{
			str->infile[i] = check_expand(str->infile[i]);
			if (str->infile[i] == NULL)
				return (return_errtx(-4, "ambiguous redirect\n"));
			if (access(str->infile[i], F_OK) != 0
				|| (access(str->infile[i], F_OK) == 0
					&& access(str->infile[i], R_OK) < 0))
				return (return_perr(-4, str->infile[i]));
			i++;
		}
		i--;
		if (str->fd_in != -2)
			return (0);
		j = open(str->infile[i], O_RDONLY);
		if (j < 0)
			return (return_perr(-4, str->infile[i]));
		str->fd_in = j;
	}
	return (0);
}

int	check_outfile_fd(t_cmd *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str->outfile != NULL)
	{
		while (str->outfile[i])
		{
			str->outfile[i] = check_expand(str->outfile[i]);
			if (str->outfile[i] && access(str->outfile[i], F_OK) == 0
				&& access(str->outfile[i], W_OK) < 0)
				{
					perror("outfile exist but not accessible\n");
					return (-4);
				}
			if (str->outfile[i] && access(str->outfile[i], F_OK) != 0)
			{
				j = open(str->outfile[i], O_CREAT, 0666);
				if (j < 0)
				{
					perror("couldn't make output file\n");
					return (-4);
				}
				close(j);
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
		{
			perror("output file open failed\n");
			return (-4);
		}
	}
	return (0);
}


int	connect_fd(t_cmd *current)
{
	// current->infile = expand_array(current->infile);
	if (check_infile_fd(current) != 0)
		return (-4);
	// current->outfile = expand_array(current->outfile);
	if (check_outfile_fd(current) != 0)
		return (-4);
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
