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

void	handle_sigint_hd(int num)
{
	(void) num;
	write(STDOUT_FILENO, "\n", 1);
}

void	ms_init(char *envp[])
{
	int	rc;
	// struct termios	termios_new;

	gl.g_status = 0;
	gl.start_env = NULL;
	gl.start_cmd = NULL;
	rl_catch_signals = 0;
	rc = tcgetattr(0, &gl.termios_save);
	if (rc)
		exit(errtx_all_free_exit(1, "get termios failed\n"));
	gl.termios_new = gl.termios_save;
	// gl.termios_new.c_lflag &= ~(ECHOCTL);
	rc = tcsetattr(0, 0, &gl.termios_new);
	if (rc)
		exit(errtx_all_free_exit(1, "set termios failed\n"));
	ft_memccpy(gl.prompt, "minishell > ", '\0', 13);
	gl.start_env = get_env(envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	
	gl.ini_fd[0] = dup(0);
	if (gl.ini_fd[0] < 0)
		exit(errtx_all_free_exit(1, "dup for initial fd failed\n"));		
	gl.ini_fd[1] = dup(1);
	if (gl.ini_fd[1] < 0)
	{
		close(gl.ini_fd[0]);
		exit(errtx_all_free_exit(1, "dup for initial fd failed\n"));
	}
}
