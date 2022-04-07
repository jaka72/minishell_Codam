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
	info->termios_new.c_iflag &= BRKINT;
	info->termios_new.c_lflag &= ~(ECHOCTL);
	info->termios_new.c_cc[VQUIT] = 0;
	info->termios_new.c_cc[VEOF] = 4;
	info->termios_new.c_cc[VINTR] = 3;
	rc = tcsetattr(0, 0, &info->termios_new);
	if (rc)
		errtext_exit("set termios failed\n");
	info->start_env = get_env(info, envp);
	signal(SIGINT, handle_sigint);
	ft_memccpy(info->prompt, "minishell > ", '\0', 13);
	// while(envp[i])
	// {
	// 	printf("%s\n", envp[i]);
	// 	i++;
	// }
	// printf("-- -- -- --\n");
	// print_env(info);
}
