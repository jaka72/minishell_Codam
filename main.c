/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 17:09:50 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

t_global	g_gl;
struct termios	g_termios_saved;

static void	free_and_read(t_source *src, int history)
{
	if (history == 1)
		add_history(src->inputline);
	if (src->inputline != NULL)
		free(src->inputline);
	src->inputline = readline("minishell > ");
}

int	main(int argc, char *argv[], char *envp[])
{
	t_source	src;
	int			ex_stat;

	(void) argc;
	(void) argv;
	src.inputline = NULL;
	ms_init(envp, &ex_stat);
	free_and_read(&src, 0);
	while (src.inputline)
	{
		if (ft_strlen(src.inputline) > 0)
		{
			if (check_syntax_errors(&src) != 0)
			{
				free_and_read(&src, 1);
				continue ;
			}
			add_history(src.inputline);
			g_gl.start_cmd = make_commands(&src);
			ex_stat = run_cmd(&ex_stat);
			free_tcmd();
		}
		free_and_read(&src, 0);
	}
	return (clean_data(ex_stat, "exit\n"));
}
