#include "../minishell.h"

void	check_infile_fd(t_cmd *str, t_infos *info)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str->infile != NULL)
	{
		while (str->infile[i])
		{
			str->infile[i] = check_expand(info, str->infile[i]);
			if (access(str->infile[i], F_OK) != 0)
				errtext_return("infile not exist\n");	// jaka: added new function. It must not exit, just return
				//errtext_exit("infile not exist\n");
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

void	check_heredoc_fd(t_cmd *str, t_infos *info)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str->heredoc != NULL)
	{
		while (str->heredoc[i])
		{
			j = make_heredoc(str->heredoc[i], info);
			i++;
		}	
		if (str->fd_in == -3)
			str->fd_in = j;
	}
}

void	check_outfile_fd(t_cmd *str, t_infos *info)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str->outfile != NULL)
	{
		while (str->outfile[i])
		{
			str->outfile[i] = check_expand(info, str->outfile[i]);
			if (str->outfile[i] && access(str->outfile[i], F_OK) == 0
				&& access(str->outfile[i], W_OK) < 0)
				errtext_exit("outfile exist but not accessible\n");
			if (str->outfile[i] && access(str->outfile[i], F_OK) != 0)
			{
				j = open(str->outfile[i], O_CREAT, 0666);
				if (j < 0)
					errtext_exit("couldn't make output file\n");
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
			errtext_exit("output file open failed\n");
	}
}

t_cmd	*manage_in_out(t_cmd *str, t_infos *info)
{
	check_infile_fd(str, info);
	check_heredoc_fd(str, info);
	check_outfile_fd(str, info);
	return (str);
}

void	connect_fd(t_cmd *current, t_infos *info)
{
	current = manage_in_out(current, info);
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
}

void	reset_fd(t_infos *info)
{
	int	fd0;
	int	fd1;

	fd0 = dup(info->ini_fd[0]);
	fd1 = dup(info->ini_fd[1]); 
	dup2(fd0, 0);
	dup2(fd1, 1);
	close(fd0);
	close(fd1);
}
