/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:11:37 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path(char *name, int *ret, int n, t_util *st_base)
{
	char	*newpath;
	t_env	*temp;

	temp = st_base->start_env;
	while (temp)
	{
		if (ft_strcmp(temp->name, name) == 0)
		{
			newpath = ft_strdup(temp->value);
			if (newpath == NULL)
			{
				write(2, "minishell: cd: malloc failed\n", 29);
				*ret = 1;
				return (NULL);
			}
			if (ft_strcmp(name, "OLDPWD") == 0 && newpath && n == 1)
			{
				write(1, newpath, ft_strlen(newpath));
				write(1, "\n", 1);
			}
			return (newpath);
		}
		temp = temp->next;
	}
	return (NULL);
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
		return (-1);
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
