#include "../minishell.h"

void	ms_init(char *envp[])
{
	g_gl.g_status = 0;
	g_gl.start_env = NULL;
	g_gl.start_cmd = NULL;
	rl_catch_signals = 0;
	if (tcgetattr(0, &g_gl.termios_save) != 0)
		exit(errtx_all_free_exit(1, "get termios failed\n"));
	ft_memccpy(g_gl.prompt, "minishell > ", '\0', 13);
	g_gl.start_env = get_env(envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	g_gl.ini_fd[0] = dup(0);
	if (g_gl.ini_fd[0] < 0)
		exit(errtx_all_free_exit(1, "dup for initial fd failed\n"));
	g_gl.ini_fd[1] = dup(1);
	if (g_gl.ini_fd[1] < 0)
	{
		close(g_gl.ini_fd[0]);
		exit(errtx_all_free_exit(1, "dup for initial fd failed\n"));
	}
}
