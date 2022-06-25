/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 16:39:36 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>

static void	ini_oldpwd(t_util *st_base)
{
	t_env	*current;

	current = st_base->start_env;
	while (current)
	{
		if (ft_strncmp("OLDPWD", current->name, 6) == 0)
		{
			if (current->value != NULL)
				free(current->value);
			current->value = malloc(sizeof(char) * 1);
			if (current->value == NULL)
				msg_and_exit("malloc for ini failed\n", 1);
			current->value[0] = '\0';
		}
		current = current->next;
	}
}

void	ms_init(char *envp[], int *ex_stat, t_util *st_base, t_source *src)
{
	st_base->start_env = NULL;
	st_base->start_cmd = NULL;
	st_base->i = 0;
	st_base->flag = 0;
	src->inputline = NULL;
	rl_catch_signals = 0;
	*ex_stat = 0;
	if (tcgetattr(0, &g_termios_saved) != 0)
		msg_and_exit("GET termios failed\n", 1);
	st_base->start_env = get_env(envp, st_base);
	ini_oldpwd(st_base);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	st_base->ini_fd[0] = dup(0);
	if (st_base->ini_fd[0] < 0)
		msg_and_exit("dup for initial fd failed\n", 1);
	st_base->ini_fd[1] = dup(1);
	if (st_base->ini_fd[1] < 0)
	{
		close(st_base->ini_fd[0]);
		msg_and_exit("dup for initial fd failed\n", 1);
	}
}
