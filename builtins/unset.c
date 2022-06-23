/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 18:07:40 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_name_delate(char *targetname)
{
	t_env	*env;
	t_env	*prev;
	int		i;

	env = g_gl.start_env;
	prev = NULL;
	i = 0;
	while (env)
	{
		if (ft_strncmp(targetname, env->name, ft_strlen(targetname)) == 0
			&& ft_strlen(targetname) == ft_strlen(env->name))
		{
			if (i == 0)
				g_gl.start_env = env->next;
			else
				prev->next = env->next;
			if (env->name)
				free(env->name);
			if (env->value)
				free(env->value);
			free(env);
			break ;
		}
		prev = env;
		env = env->next;
		i++;
	}
	return (0);
}

int	run_unset_builtin(t_cmd *cmd)
{
	int	i;
	int	ex_stat;

	i = 1;
	ex_stat = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] >= '0' && cmd->args[i][0] <= '9')
		{
			write(2, "minishell: unset: ", 18);
			write(2, cmd->args[i], ft_strlen(cmd->args[i]));
			write(2, ": not a valid identifier\n", 25);
			ex_stat = 1;
		}
		if (ft_strchr(cmd->args[i], '=') != NULL)
		{
			write(2, "minishell: unset: ", 18);
			write(2, cmd->args[i], ft_strlen(cmd->args[i]));
			write(2, ": not a valid identifier\n", 25);
			ex_stat = 1;
		}
		find_name_delate(cmd->args[i]);
		i++;
	}
	return (ex_stat);
}
