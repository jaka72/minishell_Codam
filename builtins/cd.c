/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 12:41:44 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/01 10:17:39 by jmurovec      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

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
				write(2, "minishell: error allocating memory\n", 35);
				exit (0);
			}
			return (0);
		}
		temp = temp->next;
	}
	insert_into_list(env, old_pwd);
	return (0);
}

// THERE IS A DUPLICATE OF THIS FUNCITON IN ECHO BUILTIN
char	*get_path(t_infos *info, char *name)
{
	char	*newpath;
	t_env	*temp;

	temp = info->start_env;
	while (temp)
	{
		if (ft_strcmp(temp->name, name) == 0)
		{
			printf(YEL"temp->name: [%s]\n"RES, temp->name);
			printf(YEL"temp->value: [%s]\n"RES, temp->value);
			newpath = ft_strdup(temp->value);
			printf(YEL"newpath: [%s]\n"RES, newpath);
			if (newpath == NULL)
			{
				write(2, "Minishell: Error with mallocing\n", 32);
				return (NULL);
			}
			return (newpath);
		}
		temp = temp->next;
	}
	printf(CYN"get path, not found\n"RES);
	return (NULL);
}

int	change_dir(t_infos *info, char *old_pwd, char *newpath)
{
	char	*current;
	char	buff[PATH_MAX];

	if (chdir(newpath) == -1)
	{
		write(2, "minishell: cd: No such file or directory\n", 41);
		return (1);
	}
	current = getcwd(buff, PATH_MAX);
	if (current == NULL)
		return (1);
	update_path(info->start_env, current, "PWD");
	update_path(info->start_env, old_pwd, "OLDPWD");
	return (0);
}

int	get_path_and_change_dir(t_infos *info, char *current_pwd, char *name)
{
	int		ret;
	char	*newpath;
	char	buff[PATH_MAX];

	newpath = get_path(info, name);
	if (newpath == NULL)
	{
		write(2, "minishell: cd: ", 15);
		write(2, name, ft_strlen(name));
		write(2, " not set\n", 9);
		return (1);
	}
	ret = change_dir(info, current_pwd, newpath);
	free(newpath);
	if (ret == 1)
		return (1);
	newpath = getcwd(buff, PATH_MAX);
	if (newpath == NULL)
		return (1);
	update_path(info->start_env, newpath, "PWD");
	update_path(info->start_env, current_pwd, "OLDPWD");
	return (0);
}

// Shall print some message if current_pwd is NULL ?
int	run_cd_builtin(t_cmd *cmd, t_infos *info)
{
	int		ret;
	char	buff[PATH_MAX];
	char	*current_pwd;

	ret = 0;	// jaka 1jun
	current_pwd = getcwd(buff, PATH_MAX);
	if (current_pwd == NULL)
		return (1);
	if (cmd->count_args == 1)
		ret = get_path_and_change_dir(info, current_pwd, "HOME");
	else if (cmd->count_args == 2)
	{
		if (ft_strcmp(cmd->args[1], "~") == 0)
			ret = get_path_and_change_dir(info, current_pwd, "HOME");
		else if (ft_strcmp(cmd->args[1], "-") == 0)
			ret = get_path_and_change_dir(info, current_pwd, "OLDPWD");
		else
			ret = change_dir(info, current_pwd, cmd->args[1]);
	}
	else if (cmd->count_args > 2)
		ret = print_error_too_many_args();
	if (ret == 1)
		return (1);
	return (0);
}
