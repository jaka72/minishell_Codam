#include "builtins.h"

int	insert_into_list(t_env *env, const char *value)
{
	// printf(YEL"Start insert into list\n"RES);

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


int	update_path(t_env *env, char *old_pwd, char *name)
{
	t_env *temp;

	if (old_pwd == NULL)
		return (1); // DOES IT NEED TO EXIT THE PROGRAM ???
	temp = env;
	while (temp)
	{
		//printf(RED"temp->name: [%s], value: [%s]\n", temp->name, temp->value);
		if (ft_strcmp(name, temp->name) == 0)	// OLDPWD name exists, only change name
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
	// printf(YEL"IF $NAME DOES NOT EXIST YET, CALL insert() - should only happen with OLDWPD\n"RES);
	insert_into_list(env, old_pwd);  // variable does not exist yet, insert name and value (while struct)
	return (0); 
}


/////////////// /////////////// ////////////////////////////// ///////////////  ///////////////

// THERE IS A DUPLICATE OF THIS FUNCITON IN ECHO BUILTIN
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
			// printf(YEL"temp->name: [%s]\n"RES, temp->name);
			// printf(YEL"temp->value: [%s]\n"RES, temp->value);
			newpath = ft_strdup(temp->value);
			// printf(YEL"newpath: [%s]\n"RES, newpath);
			if (newpath == NULL)
			{
				// printf("Minishell: Error with mallocing\n");
				write(2, "Minishell: Error with mallocing\n", 32);
				return (NULL);
			}
			return (newpath);	
		}
		temp = temp->next;
	}
	// printf(BLU"get path, not found\n"RES);
	return (NULL); // path not found
}




/////////////////////////////////////////////////////////////



int	get_path_and_change_dir(t_infos *info, char *current_pwd, char *name)
{
	int	ret;
	char *newpath;
	char buff[PATH_MAX];

	newpath = get_path(info, name);
	if (newpath == NULL)	// SHALL I PRINT SOME MESSAGE THAT VARIABLE $HOME IS MISSING IN ENV LIST ?
		return (1);
	ret = chdir(newpath);
	free(newpath);
	if (ret == -1)
		write(2, "minishell: cd: No such file or directory\n", 41);
		// return (1);	// shell it return without updating?
	newpath = getcwd(buff, PATH_MAX);
	if (newpath == NULL)
		return (1);

	update_path(info->start_env, newpath, "PWD");
	update_path(info->start_env, current_pwd, "OLDPWD");
	return (0);
}



int	changle_dir(t_infos *info, char *current_pwd, char *newpath)
{
	//int	ret;
	char *cwd;
	char buff[PATH_MAX];

	if (chdir(newpath) == -1)
	{
		write(2, "minishell: cd: No such file or directory\n", 41);
		return (1);	// must return, not update pwd or oldpwd
	}
	cwd = getcwd(buff, PATH_MAX);
	if (cwd == NULL)
		return (1);
	update_path(info->start_env, cwd, "PWD");
	update_path(info->start_env, current_pwd, "OLDPWD");


	return (0);
}




// Oldpwd must not change if cd path is incorrect: cd xxxx
int	run_cd_builtin(t_cmd *cmd, t_infos *info)
{
	char	buff[PATH_MAX];
	char	*current_pwd;

	current_pwd = getcwd(buff, PATH_MAX);
	if (cmd->count_args == 1)			// only cd
		get_path_and_change_dir(info, current_pwd, "HOME");
	else if (cmd->count_args == 2)
	{
		if (ft_strcmp(cmd->args[1], "~") == 0) // maybe not needed in subject
			get_path_and_change_dir(info, current_pwd, "HOME");
		else if (ft_strcmp(cmd->args[1], "-") == 0) // maybe not needed in subject
			get_path_and_change_dir(info, current_pwd, "OLDPWD");
		else	// found other path
		{
			changle_dir(info, current_pwd, cmd->args[1]);
			// if (chdir(cmd->args[1]) == -1)
			// 	write(2, "minishell: cd: No such file or directory\n", 41);
			// else
			// 	update_path(info->start_env, current_pwd);
		}
	}
	else if (cmd->count_args > 2)
		write(2, "minishell: cd: too many arguments\n", 34);
	return (0);
}

/*
cd 									ok
cd .. 								ok
cd . 								ok
cd /Users 							ok
cd /								ok
cd // 								ok
cd ////// 							ok
cd '/' 								ok
cd '//' 							ok
cd '//////'							ok
cd ./././ 							ok
cd ../../..							ok
cd '/etc' 							ok
cd '/var' 							ok
cd $PWD								ok
cd "$PWD/file_tests"				ok
cd $OLDPWD/something				ok 
cd "doesntexist" 					ok

cd "doesntexist" 2>/dev/null	 				KO  Sends error msg to bin
cd "doesntexist" "blabla" 2>/dev/null 			KO  Sends error msg to bin
cd "builtins" 2>/dev/null 						KO  Moves to bultins, no msg Too many args
cd '/////' 2>/dev/null 							KO	Moves to root, no msg Too many args
cd "wtf" 2>/dev/null 

cd ?								ok	error
cd +								ok	error
cd _								ok	error
cd woof								ok	error	No such file
cd bark bark						ok	error	Too many args


Calling $ variables without echo:
	$?			127: command not found							ok
	$PWD		bash: /home/jaka/Desktop: Is a directory					Should this be the same as Bash?
	$LOGNAME	Command 'jaka' not found

echo $PWD															KO

pwd		It shows correctly the current path   					ok
$PWD	Does not update this $ variable 							KO


*/