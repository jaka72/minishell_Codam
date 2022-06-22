/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:12:14 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_env(void)
{
	t_env	*env;

	env = g_gl.start_env;
	while (env)
	{
		if (env->value != NULL)
		{
			write(1, env->name, ft_strlen(env->name));
			write(1, "=", 1);
			write(1, env->value, ft_strlen(env->value));
			write(1, "\n", 1);
		}
		env = env->next;
	}
}

int	print_env_export(void)
{
	t_env	*env;

	env = g_gl.start_env;
	while (env)
	{
		write(1, "declare -x ", 11);
		if (env->value == NULL)
		{
			write(1, env->name, ft_strlen(env->name));
			write(1, "\n", 1);
		}
		else
		{
			write(1, env->name, ft_strlen(env->name));
			write(1, "=\"", 2);
			write(1, env->value, ft_strlen(env->value));
			write(1, "\"\n", 2);
		}
		env = env->next;
	}
	return (0);
}

int	run_env_builtin(void)
{
	print_env();
	return (0);
}

void	add_env_tolast(t_env *temp_env)
{
	t_env	*end_env;

	end_env = last_env();
	end_env->next = temp_env;
}
