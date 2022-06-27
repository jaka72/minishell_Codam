/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/26 11:54:01 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "../minishell.h"

// printf("tilda: OLDPWD [%s]\n", new_path);
// buff2[0] = 'x';
// buff2[1] = '\0';
// printf("ret = %d, buff2 [%s]\n", ret, buff2);
int	get_path_tilda_n_change_dir(char *arg, t_util *u)
{
	int		ret;
	char	buff1[PATH_MAX];
	char	buff2[PATH_MAX];
	char	*home_path;
	char	*new_path;

	home_path = get_path("HOME", &ret, 0, u);
	new_path = ft_strdup(arg + 1);
	ft_strlcpy(buff1, home_path, ft_strlen(home_path) + 1);
	ft_strlcpy(buff1 + ft_strlen(buff1), new_path,
		ft_strlen(new_path) + 1);
	ret = change_dir(getcwd(buff2, PATH_MAX), buff1, u);
	if (ret == 1)
		return (1);
	free(home_path);
	free(new_path);
	return (0);
}
// Homepath is saved into buff, then newpath is appended to buff,
//		resulting in new path

char	*get_path(char *name, int *ret, int n, t_util *u)
{
	char	*newpath;
	t_env	*temp;

	temp = u->start_env;
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
