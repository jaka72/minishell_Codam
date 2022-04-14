#include "../minishell.h"

void	handle_sigint(int num)
{
	(void) num;
	g_flag = 1;
	write(STDIN_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ms_init(t_infos *info, char *envp[])
{
	int	rc;
	int	i;

	i = 0;
	rc = tcgetattr(0, &info->termios_save);
	if (rc)
		errtext_exit("get termios failed\n");
	info->termios_new = info->termios_save;
	info->termios_new.c_lflag &= ~(ECHOCTL);
	rc = tcsetattr(0, 0, &info->termios_new);
	if (rc)
		errtext_exit("set termios failed\n");
	info->start_env = get_env(info, envp);
	signal(SIGINT, handle_sigint);
	ft_memccpy(info->prompt, "minishell > ", '\0', 13);
	info->ini_fd[0] = dup(0);
	info->ini_fd[1] = dup(1);
}
