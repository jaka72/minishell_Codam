/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:12:29 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>

static	char	*get_pwd_path(char *name, int *ret, t_util *st_base)
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
			return (newpath);
		}
		temp = temp->next;
	}
	return (NULL);
}

int	run_pwd_builtin(t_util *st_base)
{
	int		ret;
	char	buff[PATH_MAX];
	char	*current_path;

	current_path = getcwd(buff, PATH_MAX);
	if (current_path == NULL)
	{
		current_path = get_pwd_path("PWD", &ret, st_base);
		if (ret == 1)
			return (1);
		write(1, current_path, ft_strlen(current_path));
		write(1, "\n", 1);
		return (0);
	}
	else
	{
		write(1, current_path, ft_strlen(current_path));
		write(1, "\n", 1);
		return (0);
	}
}
