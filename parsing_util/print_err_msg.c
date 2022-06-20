#include "../minishell.h"

void	print_err_msg(char *err_msg)
{
	printf("\nMinishell: %s\n", err_msg);
	g_gl.g_status = SYNTAX_ERROR;
}

void	msg_and_exit(char *err_msg, int exit_code)
{
	write(2, err_msg, ft_strlen(err_msg));
	exit (exit_code);
}
