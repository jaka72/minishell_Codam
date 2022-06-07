#include "../minishell.h"

t_global	gl;

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
}

void	handle_sigquit_p(int num)
{
	(void) num;
	write(STDOUT_FILENO, "Quit: 3\n", 8);
}

void	handle_sigquit_hd(int num)
{
	(void) num;
	write(STDOUT_FILENO, "\b\b  \b\b", 6);
}

void	handle_sigint_hd(int num)
{
	(void) num;
	write(STDOUT_FILENO, "\b\b  \b\b\n", 7);
}

void	ms_init(t_infos *info, char *envp[])
{
	gl.g_status = 0;
	rl_catch_signals = 0;
	gl.start_env = get_env(envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	ft_memccpy(info->prompt, "minishell > ", '\0', 13);
	info->ini_fd[0] = dup(0);
	if (info->ini_fd[0] < 0)
	{
		free_envlist();
		errtext_exit("dup for initial fd failed\n");	
	}		
	info->ini_fd[1] = dup(1);
	if (info->ini_fd[1] < 0)
	{
		free_envlist();
		close(info->ini_fd[0]);
		errtext_exit("dup for initial fd failed\n");	
	}
}
