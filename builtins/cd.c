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

// 	CD without a path does not go to HOME ????

int	run_cd_builtin(t_cmd *cmd, t_infos *info)
{
	printf("From run_cd_builtin\n");

	char	*newpath;
	//char	*oldpath;	MAYBE NOT NECESSARY, IF "cd -" IS NOT MANDATORY
	//char	buff[PATH_MAX];
	int		ret;
	//t_env	*temp;

	//oldpath = getcwd(buff, PATH_MAX);
	//newpath = NULL;
	//printf(YEL"From cd: found oldpath: %s\n"RES, oldpath);
		// check ret, maybe needs to free
	//temp = info->start_env;
	if (cmd->count_args == 1)			// only cd
	{
		newpath = get_path(info, "HOME");
		printf(YEL"From cd: found Home: %s\n"RES, newpath);
		if (newpath == NULL)
			return (1);
		// ret = chdir(newpath);
		run_pwd_builtin();		// for testing
		ret = chdir("/home/jaka");
		run_pwd_builtin();		// for testing

		free(newpath);
		printf(GRN"ret from chdir(newpath): %d\n"RES, ret);
		if (ret == -1)
		{
			printf("minishell: cd: No such file or directory\n");
			return (1);
		}
		return (1);
	}


	else if (cmd->count_args == 2)
	{
		// if (cmd->args[1] == '-') // maybe not needed in subject
		ret = chdir(cmd->args[1]);
		printf(YEL"From cd: found some path: %s, ret: %d\n"RES, cmd->args[1], ret);
		if (ret == -1)
		{
			printf("minishell: cd: No such file or directory\n");
			return (1);
		}
		//printf(BLU"Ret from chdir(): %d\n"RES, ret);
		//return (0);
	}
	else if (cmd->count_args > 2)
	{
		printf("minishell: cd: too many arguments\n");
		return (1);
	}
	run_pwd_builtin();		// for testing
	return (1);
}
