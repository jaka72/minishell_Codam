#include "builtins.h"

// char	*get_env_name(char *name, const char *src)
// {
// 	int		i;

// 	i = 0;
// 	while (src[i] && src[i] != '=' && ft_strlen(src) < 4096) //buff_size = 4096
// 	{
// 		name[i] = src[i];
// 		i++;
// 	}
// 	name[i] = '\0';
// 	return (name);
// }



int	insert_into_list(t_env *env, const char *value)
{
	printf(YEL"Start insert into list\n"RES);

	t_env	*new;
	t_env	*temp;

	new = malloc(sizeof(t_env));
	if (new == NULL)
		return (-1); // MUST EXIT  ???
	new->value = ft_strdup(value);
	new->name = ft_strdup("OLDPWD");
	new->next = NULL;
	if (new->value == NULL || new->name == NULL)
		return (1);  // MUST EXIT ???
	temp = env;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	//printf(GRN"   name %s\n"RES, temp->name);
	return (0);
}



int		find_name_and_replace_value(t_env *env, char *old_pwd)
{
	t_env *temp;

	temp = env;
	while (temp)
	{
		//printf(RED"temp->name: [%s], value: [%s]\n", temp->name, temp->value);
		if (ft_strcmp("OLDPWD", temp->name) == 0)	// OLDPWD name exists, only change name
		{
			if (temp->value != NULL)
				free(temp->value);
			temp->value = ft_strdup(old_pwd);
			if (temp->value == NULL)
			{
				write(2, "minishell: error allocating memory\n", 35);
				exit (0); // which code to exit?
				//free_all_and_exit();
			}
			return (0); // name found and value replaced
		}
		temp = temp->next;
	}
	printf(YEL"OLDPWD DOES NOT EXIST YET, CALL insert()\n"RES);
	insert_into_list(env, old_pwd);  // variable does not exist yet, insert name and value (while struct)
	return (0); 
}




int	store_current_into_old_pwd(t_env *env, char *current_pwd)
{
	//char	buff[PATH_MAX];
	// char 	*old_pwd;

	// old_pwd = getcwd(buff, PATH_MAX);
	if (current_pwd == NULL)
		return (1); // DOES IT NEED TO EXIT THE PROGRAM ???
	find_name_and_replace_value(env, current_pwd);	// put current into $OLDPWD
	return (0);
}




char *get_path(t_infos *info, char *name)
{
	char	*newpath;
	t_env	*temp;

	temp = info->start_env;
	while (temp)
	{
		//if (strcmp(temp->name, name) == 0) // must be ft_strcmp
		if (ft_strcmp(temp->name, name) == 0)
		{
			printf(YEL"temp->name: [%s]\n"RES, temp->name);
			printf(YEL"temp->value: [%s]\n"RES, temp->value);
			newpath = ft_strdup(temp->value);
			printf(YEL"newpath: [%s]\n"RES, newpath);
			if (newpath == NULL)
			{
				printf("Minishell: Error with mallocing\n");
				return (NULL);
			}
			return (newpath);	
		}
		temp = temp->next;
	}
	printf(BLU"get path, not found\n"RES);
	return (NULL); // path not found
}





/////////////////////////////////////////////////////////////



int	get_path_and_change_dir(t_infos *info, char *current_pwd)
{
	int	ret;
	char *newpath;

	store_current_into_old_pwd(info->start_env, current_pwd);
	newpath = get_path(info, "HOME");
	if (newpath == NULL)
		return (1);
	ret = chdir(newpath);
	free(newpath);
	if (ret == -1)
		printf("minishell: cd: No such file or directory\n");
	return (0);
}



// Oldpwd must not change if cd path is incorrect: cd xxxx
int	run_cd_builtin(t_cmd *cmd, t_infos *info)
{
	//int		ret;
	char	buff[PATH_MAX];
	char	*current_pwd;

	current_pwd = getcwd(buff, PATH_MAX);
	if (cmd->count_args == 1)			// only cd
		get_path_and_change_dir(info, current_pwd);
	else if (cmd->count_args == 2)
	{
		if (ft_strcmp(cmd->args[1], "~") == 0) // maybe not needed in subject
			get_path_and_change_dir(info, current_pwd);
		else if (ft_strcmp(cmd->args[1], "-") == 0) // maybe not needed in subject
		{
			// store_old_pwd(info->start_env);
			//oldpath = getcwd(buff, PATH_MAX);
			//printf(BLU"   found dash, old path [%s]\n"RES, oldpath);
			//ret = chdir(oldpath);
			//return (1);
		}
		else	// found other path
		{
			if (chdir(cmd->args[1]) == -1)
				printf("minishell: cd: No such file or directory\n");
			else
				store_current_into_old_pwd(info->start_env, current_pwd);
		}
	}
	else if (cmd->count_args > 2)
		printf("minishell: cd: too many arguments\n");
	return (0);
}
