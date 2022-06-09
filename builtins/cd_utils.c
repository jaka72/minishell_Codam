/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 12:43:30 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/09 10:42:27 by jmurovec      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

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

int	print_error_too_many_args(void)
{
	write(2, "minishell: cd: too many arguments\n", 34);
	return (1);
}
