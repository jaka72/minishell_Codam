#include "../minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

int	return_perr(int i, char *tx)
{
	write(2, "minishell: ", 11);
	write(2, tx, ft_strlen(tx));
	perror(" ");
	return (i);
}

int	return_errtx(int i, char *tx)
{
	write(2, "minishell: ", 11);
	write(2, tx, ft_strlen(tx));
	return (i);
}

void	err_free_env_exit(char *text)
{
	perror(text);
	free_envlist();
	exit(1);
}

int	err_all_free_exit(int exitnum)
{
	free_tcmd();
	free_envlist();
	rl_clear_history();
	return (exitnum);
}

int	errtx_all_free_exit(int exitnum, char *tx)
{
	if (tx != NULL)
		perror(tx);
	return (err_all_free_exit(exitnum));
}
