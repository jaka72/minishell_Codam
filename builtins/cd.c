#include "../minishell.h"
#include <limits.h>

static	int	update_path(t_env *env, char *old_pwd, char *name)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(name, temp->name) == 0)
		{
			if (temp->value != NULL)
				free(temp->value);
			temp->value = ft_strdup(old_pwd);
			if (temp->value == NULL)
			{
				write(2, "minishell: cd: malloc failed\n", 29);
				return (1);
			}
			return (0);
		}
		temp = temp->next;
	}
	insert_oldpwd_into_list(env, old_pwd);
	return (0);
}
// variable does not exist yet, insert name and value (while struct)

static	int	change_dir(char *old_pwd, char *newpath)
{
	char	*current;
	char	buff[PATH_MAX];

	if (chdir(newpath) == -1)
	{
		write(2, "minishell: cd: ", 15);
		write(2, newpath, ft_strlen(newpath));
		write(2, ": No such file or directory\n", 28);
		return (1);
	}
	current = getcwd(buff, PATH_MAX);
	if (current == NULL)
		return (1);
	if (update_path(g_gl.start_env, current, "PWD") != 0)
		return (1);
	if (update_path(g_gl.start_env, old_pwd, "OLDPWD") != 0)
		return (1);
	return (0);
}

static	int	get_path_and_change_dir(char *current_pwd, char *name, int n)
{
	int		ret;
	char	*newpath;
	char	buff[PATH_MAX];

	ret = 0;
	newpath = get_path(name, &ret, n);
	if (ret == 1)
		return (1);
	if (newpath == NULL)
		return (print_msg_var_not_set(name));
	ret = change_dir(current_pwd, newpath);
	free(newpath);
	if (ret == 1)
		return (1);
	newpath = getcwd(buff, PATH_MAX);
	if (newpath == NULL)
		return (1);
	if (update_path(g_gl.start_env, newpath, "PWD") != 0)
		return (1);
	if (update_path(g_gl.start_env, current_pwd, "OLDPWD") != 0)
		return (1);
	return (0);
}

int	if_folder_deleted(int *ret, t_cmd *cmd, char *current_pwd)
{
	if (current_pwd == NULL)
	{
		if (ft_strcmp(cmd->args[1], "..") == 0
			|| ft_strcmp(cmd->args[1], "-") == 0)
		{
			*ret = get_path_and_change_dir(current_pwd, "OLDPWD", 0);
			if (*ret == 1)
				return (1);
		}
	}
	return (0);
}

int	run_cd_builtin(t_cmd *cmd)
{
	int		ret;
	char	buff[PATH_MAX];
	char	*current_pwd;

	ret = 0;
	current_pwd = getcwd(buff, PATH_MAX);
	if (current_pwd == NULL)
		return (if_folder_deleted(&ret, cmd, current_pwd));
	if (count_elems(cmd->args) == 1)
		ret = get_path_and_change_dir(current_pwd, "HOME", 1);
	else if (count_elems(cmd->args) >= 2)
	{
		if (ft_strcmp(cmd->args[1], "~") == 0)
			ret = get_path_and_change_dir(current_pwd, "HOME", 1);
		else if (ft_strcmp(cmd->args[1], "-") == 0)
			ret = get_path_and_change_dir(current_pwd, "OLDPWD", 1);
		else
			ret = change_dir(current_pwd, cmd->args[1]);
	}
	if (ret == 1)
		return (1);
	return (0);
}
