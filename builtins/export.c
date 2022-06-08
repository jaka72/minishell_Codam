//#include "minishell.h"
#include "builtins.h"

void	print_env_export()
{
	t_env	*env;

	env = gl.start_env;
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
			env->name[i] = '\0';
			// printf("%s\n", env->name);
			env->value = malloc(sizeof(char)*(w - i));
			
			if (env->value == NULL)
				errtext_exit("for value malloc failed");
			ft_strlcpy(env->value, &s[i + 1], (w - i));
			env->value[w - i] = '\0';
			env->next = NULL;
			// printf("%s\n", env->value);
			return(env);
		}
		i++;
	}
	return (NULL);
}

int	check_valid_identifier(char *text)
{
	int	i;

	i = 0;
	if (text[0] <= '9' && text[0] >= '0')
		return (1);
	while(text[i])
	{
		if (text[i] == '[' || text[i] == ']' || text[i] == '<' || text[i] == '>' || text[i] == '|')
			return (2);
		i++;
	}
	return(0);
}

int	write_identifier_error(char *text)
{
	write(2, "'", 1);
	write(2, text, ft_strlen(text));
	write(2, "' : not a valid identifier\n", 28);
	return (1);
}

int	run_export_builtin(t_cmd *cmd)
{
	int		i;
	t_env	*temp_env;
	t_env	*end_env;

	i = 1;
	if (cmd->args[1] == NULL)
		print_env_export();
	else
	{
		while(cmd->args[i])
		{
			if (cmd->args[1][0] == '=')
				return(write_identifier_error(cmd->args[i]));
			if (ft_strchr(cmd->args[i], '=') != NULL)
			{
				temp_env = malloc(sizeof(t_env) * 1);
				if (temp_env == NULL)
					errtext_exit("for temp_env malloc failed");
				temp_env = find_and_split(cmd->args[i], '=', temp_env);
				if (check_valid_identifier(temp_env->name) != 0)
					return(write_identifier_error(cmd->args[i]));
				find_name_delate(temp_env->name); // to find if the same name variable is exist 
				end_env = last_env(gl.start_env);
				end_env->next = temp_env;
			}
			i++;
		}
	}
	return (0);
}
