/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 17:31:57 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>

static void	ini_oldpwd(void)
{
	t_env	*current;

	current = g_gl.start_env;
	while (current)
	{
		if (ft_strncmp("OLDPWD", current->name, 6) == 0)
		{
			if (current->value != NULL)
				free(current->value);
			current->value = malloc(sizeof(char) * 1);
			if (current->value == NULL)
				exit(errtx_all_free_exit(1, "malloc for ini failed\n"));
			current->value[0] = '\0';
		}
		current = current->next;
	}
}

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
	ini_oldpwd();
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
