#include "../minishell.h"

t_env	*last_env(void)
{
	t_env	*last;

	last = gl.start_env;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

int	count_env(void)
{
	int		i;
	t_env	*current;

	i = 0;
	current = gl.start_env;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
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
			exit(errtx_all_free_exit(1, "malloc for env failed\n"));
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
				exit(errtx_all_free_exit(1, "malloc for env failed\n"));
			}
			env->value = ft_memcpy(env->value, &envtext[i + 1], ft_strlen(envtext) - i);
			env->value[ft_strlen(envtext) - i] = '\0';
		}
	}
	return (env);
}

t_env	*get_env(char *envp[])
{
	int		i;
	t_env	*temp_env;
	t_env	*current_env;

	temp_env = NULL;
	current_env = NULL;
	i = 0;
	while (envp[i])
	{
		temp_env = malloc(sizeof(t_env));
		if (temp_env == NULL)
			exit(errtx_all_free_exit(1, "malloc for env failed\n"));
		temp_env->name = NULL;
		temp_env->value = NULL;
		temp_env->next = NULL;
		temp_env = get_name_value(temp_env, envp[i]);
		if (temp_env == NULL)
			err_free_env_exit("getting env failed\n");
		if (gl.start_env == NULL)
			gl.start_env = temp_env;
		else
		{
			current_env = last_env();
			current_env->next = temp_env;
		}
		i++;
	}
	return (gl.start_env);
}

char	**ft_realloc_i(char **str, int i)
{
	char	**temp;
	int		j;

	j = 0;
	temp = malloc(sizeof(char **) * i);
	if (temp == NULL)
		return (NULL);
	while (str[j] && j < i)
	{
		temp[j] = str[j];
		j++;
	}
	temp[i - 1] = NULL;
	return (temp);
}

char	**get_env_array(void)
{
	char	**envs;
	char	*temp;
	t_env	*current;
	int		i;

	i = 0;
	envs = malloc(sizeof(char **) * 1);
	if (envs == NULL)
		exit(errtx_all_free_exit(1, "malloc for envs failed\n"));
	envs[0] = NULL;
	temp = NULL;
	current = gl.start_env;
	while (current)
	{
		envs = ft_add_str(envs);
		if (envs == NULL)
			return (NULL);
		temp = malloc((ft_strlen(current->name) + ft_strlen(current->value) + 2));
		if (temp == NULL)
			exit(errtx_all_free_exit(1, "malloc for envs failed\n"));
		ft_memcpy(temp, current->name, ft_strlen(current->name));
		ft_memcpy(&temp[ft_strlen(current->name)], "=", 1);
		ft_memcpy(&temp[ft_strlen(current->name) + 1], current->value, ft_strlen(current->value) + 1);
		envs[i] = temp;
		envs[i + 1] = NULL;
		current = current->next;
		i++; 
	}
	return(envs);
}
