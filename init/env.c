/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 19:27:08 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*get_env(char *envp[], t_util *st_base)
{
	int		i;
	t_env	*temp_env;
	t_env	*current_env;

	temp_env = NULL;
	current_env = NULL;
	i = 0;
	while (envp[i])
	{
		temp_env = init_tempenv();
		temp_env = get_name_value(temp_env, envp[i]);
		if (temp_env == NULL)
			msg_and_exit("getting env failed\n", 1);
		if (st_base->start_env == NULL)
			st_base->start_env = temp_env;
		else
		{
			current_env = last_env(st_base);
			current_env->next = temp_env;
		}
		i++;
	}
	return (st_base->start_env);
}

static char	**make_envstr(t_env *current, char **envs, int *i)
{
	char	*temp;

	temp = malloc((ft_strlen(current->name) + ft_strlen(current->value) + 2));
	if (temp == NULL)
	{
		free_strings(envs);
		msg_and_exit("malloc for envs failed\n", 1);
	}		
	ft_memcpy(temp, current->name, ft_strlen(current->name));
	ft_memcpy(&temp[ft_strlen(current->name)], "=", 1);
	ft_memcpy(&temp[ft_strlen(current->name) + 1],
		current->value, ft_strlen(current->value) + 1);
	envs[*i] = temp;
	envs[*i + 1] = NULL;
	return (envs);
}

char	**get_env_array(t_util *st_base)
{
	char	**envs;
	t_env	*current;
	int		i;

	i = 0;
	envs = malloc(sizeof(char **) * 1);
	if (envs == NULL)
		msg_and_exit("malloc for envs failed\n", 1);
	envs[0] = NULL;
	current = st_base->start_env;
	while (current)
	{
		if (current->name && current->value)
		{
			envs = ft_add_str(envs);
			if (envs == NULL)
				return (NULL);
			envs = make_envstr(current, envs, &i);
		}
		current = current->next;
		i++;
	}
	return (envs);
}
