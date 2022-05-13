#include "builtins.h"



// int			env_add(const char *value, t_env *env)
// {
// 	t_env	*new;
// 	t_env	*tmp;

// 	if (env && env->value == NULL)
// 	{
// 		env->value = ft_strdup(value);
// 		return (0);
// 	}
// 	if (!(new = malloc(sizeof(t_env))))
// 		return (-1);

// 	new->value = ft_strdup(value);
// 	while (env && env->next && env->next->next)
// 		env = env->next;
// 	tmp = env->next;
// 	env->next = new;
// 	new->next = tmp;
// 	return (0);
// }


// char		*get_env_name(char *dest, const char *src)
// {
// 	int		i;

// 	i = 0;
// 	while (src[i] && src[i] != '=' && ft_strlen(src) < 4096) //buff_size = 4096
// 	{
// 		dest[i] = src[i];
// 		i++;
// 	}
// 	dest[i] = '\0';
// 	return (dest);
// }


// int			is_in_env(t_env *env, char *args)
// {
// 	char	var_name[4096];	// # define BUFF_SIZE 4096
// 	char	env_name[4096];

// 	get_env_name(var_name, args);
// 	while (env && env->next)
// 	{
// 		get_env_name(env_name, env->value);
// 		if (ft_strcmp(var_name, env_name) == 0)
// 		{
// 			ft_memdel(env->value);
// 			env->value = ft_strdup(args);
// 			return (1);
// 		}
// 		env = env->next;
// 	}
// 	return (0);
// }



// int	store_old_pwd(void)
// {
// 	char	buff[PATH_MAX];
// 	char	*cur_pwd;
// 	char 	*old_pwd;

// 	cur_pwd = getcwd(buff, PATH_MAX);
// 	if (cur_pwd == NULL)
// 		return (1);
	
// 	old_pwd = ft_strjoin("OLDPWD=", cur_pwd);
// 	if (old_pwd == NULL)
// 		return (1);

// 	printf(YEL"Store_old_pwd [%s]\n", old_pwd);


// 	return (0);
// }


char	*get_path(t_infos *info, char *name)
{
	char	*path;
	t_env	*temp;

	temp = info->start_env;
	while (temp)
	{
		if (strcmp(temp->name, name) == 0) // must be ft_strcmp
		{
			path = ft_strdup(temp->value);
				// check ret, if malloced
			return (path);	
		}
		temp = temp->next;
	}
	return (NULL);	
}



int	run_cd_builtin(t_cmd *cmd, t_infos *info)
{
	printf("From run_cd_builtin\n");

	char	*newpath;
	char	*oldpath;
	char	buff[PATH_MAX];
	//t_env	*temp;

	oldpath = getcwd(buff, PATH_MAX);
	printf(YEL"From cd: found oldpath: %s\n"RES, oldpath);
		// check ret, maybe needs to free
	//temp = info->start_env;
	if (cmd->count_args > 2)
	{
		printf("Error CD, too many arguments\n");
		return (1);
	}
	if (cmd->count_args == 1)			// only cd
	{
		newpath = get_path(info, "HOME");
		printf(YEL"From cd: found Home: %s\n"RES, newpath);
			// check ret, if NULL ...
		chdir(newpath);
		return (0);
	}
	else if (cmd->count_args == 2)
	{
		// if (cmd->args[1] == '-') // maybe not needed in subject
		chdir(cmd->args[1]);
	}



	return (0);
}	