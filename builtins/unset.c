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

static	void	free_env_item(t_env *env)
{
	if (env->name)
		free(env->name);
	if (env->value)
		free(env->value);
	free(env);
}

int	find_name_delate(char *targetname, t_util *st_base)
{
	t_env	*env;
	t_env	*prev;

	env = st_base->start_env;
	prev = NULL;
	st_base->i = 0;
	while (env)
	{
		if (ft_strncmp(targetname, env->name, ft_strlen(targetname)) == 0
			&& ft_strlen(targetname) == ft_strlen(env->name))
		{
			if (st_base->i == 0)
				st_base->start_env = env->next;
			else
				prev->next = env->next;
			free_env_item(env);
			break ;
		}
		prev = env;
		env = env->next;
		(st_base->i)++;
	}
	return (0);
}

int	run_unset_builtin(t_cmd *cmd, t_util *st_base)
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
		find_name_delate(cmd->args[i], st_base);
		i++;
	}
	return (ex_stat);
}
