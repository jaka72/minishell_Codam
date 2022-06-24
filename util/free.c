/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:15:50 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_envlist(t_util *st_base)
{
	t_env	*env;
	t_env	*temp_env;

	env = st_base->start_env;
	while (env)
	{
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		temp_env = env;
		env = env->next;
		free(temp_env);
	}
	st_base->start_env = NULL;
}

int	free_strings(char **strs)
{
	int	i;

	i = 0;
	if (strs == NULL)
		return (-1);
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	strs = NULL;
	return (0);
}

void	free_tcmd(t_util *st_base)
{
	t_cmd	*current;
	t_cmd	*temp;

	current = st_base->start_cmd;
	while (current)
	{
		free_strings(current->args);
		free_strings(current->heredoc);
		free_strings(current->files);
		temp = current;
		current = current->next;
		free(temp);
	}
	st_base->start_cmd = NULL;
}
