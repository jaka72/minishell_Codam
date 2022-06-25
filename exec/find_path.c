/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   find_path.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 19:28:22 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_find_env_pathnum(char *envp[])
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = &envp[i][5];
			return (path);
		}
		i++;
	}
	return (NULL);
}

static char	*ft_make_binpath(int i, char *path, char *cmd)
{
	int		j;
	int		k;
	char	*bin;

	j = 0;
	k = 0;
	bin = malloc(i + 2 + ft_strlen(cmd));
	if (bin == NULL)
		return (NULL);
	while (j < i)
	{
		bin[j] = path[j];
		j++;
	}
	bin[j] = '/';
	j++;
	while (k < (int)ft_strlen(cmd))
	{
		bin[j + k] = cmd[k];
		k++;
	}
	bin[j + k] = '\0';
	return (bin);
}

char	*ft_findshell_path(char *cmd, char *envp[])
{
	int		i;
	char	*path;
	char	*bin;

	i = 0;
	path = ft_find_env_pathnum(envp);
	while (path != NULL && path[i] != '\0')
	{
		while (path[i] != ':' && path[i] != '\0')
			i++;
		if (path[i] == ':' || path[i] == '\0')
		{
			bin = ft_make_binpath(i, path, cmd);
			if (bin == NULL)
				return (NULL);
			if (access(bin, F_OK) == 0)
				return (bin);
			free(bin);
			if (path[i] == '\0')
				return (NULL);
			path = &path[i + 1];
			i = 0;
		}
	}
	return (NULL);
}
