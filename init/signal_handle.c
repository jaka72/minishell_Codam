#include "../minishell.h"
#include <readline/readline.h>
#include <sys/ioctl.h>

void	handle_sigint(int num)
{
	(void) num;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigquit(int num)
{
	(void) num;
}

void	handle_sigint_p(int num)
{
	int	i;

	(void) num;
	write(STDOUT_FILENO, "\n", 1);
	reset_fd_sig();
	i = tcsetattr(0, 0, &g_gl.termios_save);
	if (i)
		exit(errtx_all_free_exit(1, "set termios failed\n"));
}

void	handle_sigquit_p(int num)
{
	int	i;

	(void) num;
	write(STDOUT_FILENO, "Quit: 3\n", 8);
	reset_fd_sig();
	i = tcsetattr(0, 0, &g_gl.termios_save);
	if (i)
		exit(errtx_all_free_exit(1, "set termios failed\n"));
}

void	handle_sigint_hd(int num)
{
	int	i;

	(void) num;
	write(STDOUT_FILENO, "\n", 1);
	i = tcsetattr(0, 0, &g_gl.termios_save);
	if (i)
		exit(errtx_all_free_exit(1, "set termios failed\n"));
}
