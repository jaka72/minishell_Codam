#include "../minishell.h"

void	handle_sigint(int num)
{
	(void) num;
	write(STDIN_FILENO, "\n", 1);
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
	write(STDIN_FILENO, "\n", 1);
}

void	handle_sigquit_p(int num)
{
	// (void) num;
	write(num, "Quit: 3\n", 8);
}

void	ms_init(t_infos *info, char *envp[])
{
	int	rc;

	rl_catch_signals = 0;
	rc = tcgetattr(0, &info->termios_save);
	if (rc)
		errtext_exit("get termios failed\n");
	info->termios_new = info->termios_save;
	rc = tcsetattr(0, 0, &info->termios_new);
	if (rc)
		errtext_exit("set termios failed\n");
	info->start_env = get_env(info, envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	ft_memccpy(info->prompt, "minishell > ", '\0', 13);
	info->ini_fd[0] = dup(0);
	if (info->ini_fd[0] < 0)
	{
		free_envlist(info);
		errtext_exit("dup for initial fd failed\n");	
	}		
	info->ini_fd[1] = dup(1);
	if (info->ini_fd[1] < 0)
	{
		free_envlist(info);
		close(info->ini_fd[0]);
		errtext_exit("dup for initial fd failed\n");	
	}
}
