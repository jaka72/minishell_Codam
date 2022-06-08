#include "../minishell.h"

int	find_name_delate(char *targetname)
{
	t_env	*env;
	t_env	*prev;

	env = gl.start_env;
	prev = NULL;
	while (env)
	{
		if (ft_strncmp(targetname, env->name, ft_strlen(targetname) + 1) == 0
			&& ft_strlen(targetname) == ft_strlen(env->name))
		{
			// printf("targetname is %s found %s %s!!\n", targetname, env->name, env->value);
			if (prev == NULL)
				gl.start_env = env->next;
			else
				prev->next = env->next;
			free(env);
			break;
		}
		prev = env;
		env = env->next;
	}
	return (0);
}

int	run_unset_builtin(t_cmd *cmd)
{
	int	i;
	//t_env	*env;
	//t_env	*prev;

	i = 1;
	//env = gl.start_env;
	//prev = NULL;
	while (cmd->args[i])
	{
		find_name_delate(cmd->args[i]);
		//env = gl.start_env;
		i++;
	}
	return (0);
}
