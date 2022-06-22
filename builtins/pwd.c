#include "../minishell.h"
#include <limits.h>

static	char	*get_pwd_path(char *name, int *ret)
{
	char	*newpath;
	t_env	*temp;

	temp = g_gl.start_env;
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

int	run_pwd_builtin(void)
{
	int		ret;
	char	buff[PATH_MAX];
	char	*current_path;

	current_path = getcwd(buff, PATH_MAX);
	if (current_path == NULL)
	{
		current_path = get_pwd_path("PWD", &ret);
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
