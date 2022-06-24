/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_handle.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 16:12:57 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
	i = tcsetattr(0, 0, &g_termios_saved);
	if (i)
		msg_and_exit("set termios failed\n", 1);
}

void	handle_sigquit_p(int num)
{
	int	i;

	(void) num;
	write(STDOUT_FILENO, "Quit: 3\n", 8);
	i = tcsetattr(0, 0, &g_termios_saved);
	if (i)
		msg_and_exit("set termios failed\n", 1);
}

void	handle_sigint_hd(int num)
{
	int	i;

	(void) num;
	write(STDOUT_FILENO, "\n", 1);
	i = tcsetattr(0, 0, &g_termios_saved);
	if (i)
		msg_and_exit("set termios failed\n", 1);
}
