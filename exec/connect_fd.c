#include "../minishell.h"

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

void	check_close_fd(int fdin, int fdout)
{
	if (fdin > 2)
		close(fdin);
	if (fdout > 2)
		close(fdout);
}

void	reset_fd_sig(void)
{
	int	fd0;
	int	fd1;

	fd0 = dup(g_gl.ini_fd[0]);
	fd1 = dup(g_gl.ini_fd[1]);
	dup2(fd0, 0);
	dup2(fd1, 1);
	close(fd0);
	close(fd1);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}
