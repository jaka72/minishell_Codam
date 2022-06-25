/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_util.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:13:37 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*last_env(t_util *st_base)
{
	t_env	*last;

	last = st_base->start_env;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

// static void	free_errtx_all_free_exit(t_env *env, char *envname)
// {
// 	if (envname != NULL)
// 		free(envname);
// 	if (env != NULL)
// 		free(env);
// 	exit(errtx_all_free_exit(1, "malloc for env failed\n"));
// }

t_env	*get_name_value(t_env *env, char *envtext)
{
	int	i;

	i = 0;
	while (envtext[i] != '=' && envtext[i] != '\0')
		i++;
	if (envtext[i] == '=' || envtext[i] == '\0')
	{
		env->name = malloc(i + 1);
		if (env->name == NULL)
			msg_and_exit("malloc for env failed\n", 1);
		env->name = ft_memcpy(env->name, envtext, i);
		env->name[i] = '\0';
		if (i < (int)ft_strlen(envtext))
		{
			env->value = malloc(ft_strlen(envtext) - i + 1);
			if (env->value == NULL)
				msg_and_exit("malloc for env failed\n", 1);
			env->value = ft_memcpy(env->value,
					&envtext[i + 1], ft_strlen(envtext) - i);
			env->value[ft_strlen(envtext) - i] = '\0';
		}
	}
	return (env);
}

t_env	*init_tempenv(void)
{
	t_env	*temp_env;

	temp_env = malloc(sizeof(t_env));
	if (temp_env == NULL)
		msg_and_exit("malloc for env failed\n", 1);
	temp_env->name = NULL;
	temp_env->value = NULL;
	temp_env->next = NULL;
	return (temp_env);
}
