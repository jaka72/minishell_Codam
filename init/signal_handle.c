#include "../minishell.h"
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
	(void) num;
	write(STDOUT_FILENO, "\n", 1);
	reset_fd_sig();
}

void	handle_sigquit_p(int num)
{
	(void) num;

	write(STDOUT_FILENO, "Quit: 3\n", 8);
	// reset_fd_sig();
}

void	handle_sigint_hd(int num)
{
	(void) num;
	write(STDOUT_FILENO, "\n", 1);
}
