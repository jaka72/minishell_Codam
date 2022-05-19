#include "minishell.h"

void	print_env_export(t_infos *info)
{
	t_env	*env;

	env = info->start_env;
	while (env->next)
	{
		printf("declare -x ");
		printf("%s=\"%s\"\n", env->name, env->value);
		env = env->next;
	}
	printf("declare -x ");
	printf("%s=\"%s\"\n", env->name, env->value);
}

t_env	*find_and_split(const char *s, char c, t_env *env)
{
	int	i;
	int	w;

	i = 0;
	w = ft_strlen(s);
	while (s[i] != '\0' || (c == '\0' && s[i] == '\0'))
	{
		if (s[i] == c)
		{
			
			env->name = malloc(sizeof(char)*(i + 1));
			if (env->name == NULL)
				errtext_exit("for name malloc failed");
			ft_strlcpy(env->name, s, i + 1);
			printf("find!\n");	
			env->name[i] = '\0';
			printf("%s\n", env->name);
			env->value = malloc(sizeof(char)*(w - i));
			
			if (env->value == NULL)
				errtext_exit("for value malloc failed");
			ft_strlcpy(env->value, &s[i + 1], (w - i));
			env->value[w - i] = '\0';
			env->next = NULL;
			printf("%s\n", env->value);
			return(env);
		}
		i++;
	}
	return (NULL);
}

int	run_export_builtin(t_cmd *cmd, t_infos *info)
{
	int		i;
	t_env	*temp_env;
	t_env	*end_env;

	i = 1;
	temp_env = malloc(sizeof(t_env) * 1);
	if (temp_env == NULL)
		errtext_exit("for temp_env malloc failed");
	if (cmd->args[1] == NULL)
	{
		print_env_export(info);
	}
	else{
		while(cmd->args[i])
		{
			printf("%s\n", cmd->args[i]);
			if (cmd->args[1][0] == '=')
			{
				printf("%s : not a valid identifier\n", cmd->args[i]);
				return (1);
			}
			if (ft_strchr(cmd->args[i], '=') != NULL)
			{
				temp_env = find_and_split(cmd->args[i], '=', temp_env);
				find_name_delate(temp_env->name, info); // to find if the same name variable is exist 
				end_env = last_env(info->start_env);
				end_env->next = temp_env;
			}
			i++;
		}
	}

	return (0);
}
