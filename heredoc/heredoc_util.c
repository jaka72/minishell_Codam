#include "../minishell.h"

void	init_pid_sig_hd(t_pid *pidinfo)
{
	int	i;

	pidinfo->status = 0;
	pidinfo->newpipe[0] = 0;
	pidinfo->newpipe[1] = 0;
	pidinfo->newpipe[2] = 0;
	pidinfo->temp_cmd = g_gl.start_cmd;
	signal(SIGINT, handle_sigint_hd);
	signal(SIGQUIT, handle_sigquit);
	g_gl.termios_new.c_lflag &= ~(ECHOCTL);
	i = tcsetattr(0, 0, &g_gl.termios_new);
	if (i)
		exit(errtx_all_free_exit(1, "set termios failed\n"));
}

void	reset_termios(void)
{
	int	i;

	g_gl.termios_new.c_lflag |= (ECHOCTL);
	i = tcsetattr(0, 0, &g_gl.termios_new);
	if (i)
		exit(errtx_all_free_exit(1, "set termios failed\n"));
}

int	reset_fd_sig_close(int fd)
{
	reset_fd_sig();
	close(fd);
	return (-1);
}
