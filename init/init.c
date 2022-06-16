#include "../minishell.h"

t_global	gl;

void	ms_init(char *envp[])
{
	gl.g_status = 0;
	gl.start_env = NULL;
	gl.start_cmd = NULL;
	rl_catch_signals = 0;
	if (tcgetattr(0, &gl.termios_save) != 0)
		exit(errtx_all_free_exit(1, "get termios failed\n"));
	gl.termios_new = gl.termios_save;
	if (tcsetattr(0, 0, &gl.termios_new) != 0)
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
