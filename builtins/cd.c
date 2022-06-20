#include "../minishell.h"

// variable does not exist yet, insert name and value (while struct)
// printf(YEL"IF $NAME DOES NOT EXIST YET, CALL 
//			insert() - should only happen with OLDWPD\n"RES);
// IF MALLOC FAILS, WHICH CODE TO EXIT ??
// free_all_and_exit();
int	update_path(t_env *env, char *old_pwd, char *name)
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
				//exit (0);
			}
			return (0);
		}
		temp = temp->next;
	}
	insert_oldpwd_into_list(env, old_pwd);
	return (0);
}

// THERE IS A DUPLICATE OF THIS FUNCITON IN ECHO BUILTIN
//     printf(YEL"temp->value: [%s]\n"RES, temp->value);
char	*get_path(char *name, int *ret)
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
			if (ft_strcmp(name, "OLDPWD") == 0 && newpath)
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

int	change_dir(char *old_pwd, char *newpath)
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

 // get_path(): 2 errors: malloc failed or var not set.
int	get_path_and_change_dir(char *current_pwd, char *name)
{
	int		ret;
	char	*newpath;
	char	buff[PATH_MAX];

	ret = 0;
	newpath = get_path(name, &ret);
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

// Shall print some message if current_pwd is NULL ?
int	run_cd_builtin(t_cmd *cmd)
{
	int		ret;
	char	buff[PATH_MAX];
	char	*current_pwd;

	ret = 0;
	current_pwd = getcwd(buff, PATH_MAX);
	if (current_pwd == NULL)
		return (1);
	if (count_elems(cmd->args) == 1)
		ret = get_path_and_change_dir(current_pwd, "HOME");
	else if (count_elems(cmd->args) == 2)
	{
		if (ft_strcmp(cmd->args[1], "~") == 0)
			ret = get_path_and_change_dir(current_pwd, "HOME");
		else if (ft_strcmp(cmd->args[1], "-") == 0)
			ret = get_path_and_change_dir(current_pwd, "OLDPWD");
		else
			ret = change_dir(current_pwd, cmd->args[1]);
	}
	else if (count_elems(cmd->args) > 2)
		ret = print_error_too_many_args();
	if (ret == 1)
		return (1);
	return (0);
}
