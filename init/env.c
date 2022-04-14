#include "../minishell.h"

t_env	*last_env(t_env *start_env)
{
	t_env	*last;

	last = start_env;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

int	count_env(t_env *start_env)
{
	int	i;
	t_env	*current;

	i = 0;
	current = start_env;
	while(current)
	{
		i++;
		current = current->next;
	}
	return(i);
}

t_env	*get_name_value(t_env *env, char *envtext)
{
	int	i;

	i = 0;
	while (envtext[i] != '=' && envtext[i] != '\0')
		i++;
	if (envtext[i] == '=' || envtext[i] == '\0')
	{
		env->name = malloc(i + 1);
		if (env->name == NULL)
		{
			free(env);
			return (NULL);
		}
		env->name = ft_memcpy(env->name, envtext, i);
		env->name[i] = '\0';
		if (i < (int)ft_strlen(envtext))
		{
			env->value = malloc(ft_strlen(envtext) - i + 1);
			if (env->value == NULL)
			{
				free(env->name);
				free(env);
				return (NULL);
			}
			env->value = ft_memcpy(env->value, &envtext[i + 1], ft_strlen(envtext) - i);
			env->value[ft_strlen(envtext) - i] = '\0';
		}
	}
	return (env);
}

t_env	*get_env(t_infos *infos, char *envp[])
{
	int		i;
	t_env	*temp_env;
	t_env	*current_env;

	temp_env = NULL;
	current_env = NULL;
	infos->start_env = NULL;
	i = 0;
	while (envp[i])
	{
		temp_env = malloc(sizeof(t_env));
		if (temp_env == NULL)
		{
			if (infos->start_env == NULL)
				errtext_exit("malloc for env failed\n");
			else
				err_free_env_exit(infos, "malloc for env failed\n");
		}
		temp_env->name = NULL;
		temp_env->value = NULL;
		temp_env->next = NULL;
		temp_env = get_name_value(temp_env, envp[i]);
		if (temp_env == NULL)
			err_free_env_exit(infos, "getting env failed\n");
		if (infos->start_env == NULL)
			infos->start_env = temp_env;
		else
		{
			current_env = last_env(infos->start_env);
			current_env->next = temp_env;
		}
		i++;
	}
	return (infos->start_env);
}

char	**get_env_array(t_env *start_env)
{
	char	**envs;
	char	*temp;
	t_env	*current;
	int		i;

	i = 0;
	envs = malloc(sizeof(char **) * 1);
	if (envs == NULL)
		errtext_exit("malloc for envs failed\n");
	temp = NULL;
	current = start_env;
	while (current)
	{
		temp = malloc((ft_strlen(current->name) + ft_strlen(current->value) + 2));
		ft_memcpy(temp, current->name, ft_strlen(current->name));
		ft_memcpy(&temp[ft_strlen(current->name)], "=", 1);
		ft_memcpy(&temp[ft_strlen(current->name) + 1], current->value, ft_strlen(current->value) + 1);
		envs[i] = temp;
		current = current->next;
		i++; 
	}
	envs[i] = NULL;
	return(envs);
}
