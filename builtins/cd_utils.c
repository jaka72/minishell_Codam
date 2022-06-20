/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 12:43:30 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/20 08:54:16 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include "builtins.h"
#include "../minishell.h"

int	print_error_too_many_args(void)
{
	write(2, "minishell: cd: too many arguments\n", 34);
	return (1);
}

int	print_msg_var_not_set(char *name)
{
	write(2, "minishell: cd: ", 15);
	write(2, name, ft_strlen(name));
	write(2, " not set\n", 9);
	return (1);
}

int	insert_oldpwd_into_list(t_env *env, const char *value)
{
	t_env	*new;
	t_env	*temp;

	new = malloc(sizeof(t_env));
	if (new == NULL)
		return (-1); // 	NEEDS TO GIVE MESSAGE IF MALLOC FAILED, FREE AND AND EXIT !!!
	new->value = ft_strdup(value);
	new->name = ft_strdup("OLDPWD");
	new->next = NULL;
	if (new->value == NULL || new->name == NULL)
		return (1);
	temp = env;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (0);
}
