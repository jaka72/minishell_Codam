#include "../minishell.h"

static t_env	*find_and_split(const char *s, char c, t_env *env)
{
	int	i;
	int	w;

	i = 0;
	w = ft_strlen(s);
	while (s[i] != '\0' || (c == '\0' && s[i] == '\0'))
	{
		if (s[i] == c)
		{
			env->name = malloc(sizeof(char) * (i + 1));
			if (env->name == NULL)
				exit(errtx_all_free_exit(1, "for env name malloc failed\n"));
			ft_strlcpy(env->name, s, i + 1);
			env->name[i] = '\0';
			env->value = malloc(sizeof(char) * (w - i));
			if (env->value == NULL)
				exit(errtx_all_free_exit(1, "for env value malloc failed\n"));
			ft_strlcpy(env->value, &s[i + 1], (w - i));
			env->value[w - i - 1] = '\0';
			env->next = NULL;
			return (env);
		}
		i++;
	}
	return (NULL);
}

static int	check_valid_identifier(char *text)
{
	int	i;

	i = 0;
	if (text[0] <= '9' && text[0] >= '0')
		return (1);
	while (text[i])
	{
		if (text[i] == '<' || text[i] == '>' || text[i] == '|')
			return (2);
		if (text[i] == '\'' || text[i] == '\"')
			return (3);
		if (text[i] == '[' || text[i] == ']' || text[i] == ':')
			return (4);
		if (text[i] == '=')
			break ;
		i++;
	}
	return (0);
}

static int	write_identifier_error(char *text)
{
	write(2, "minishell: export: \'", 20);
	write(2, text, ft_strlen(text));
	write(2, "' : not a valid identifier\n", 28);
	return (1);
}

static	void	env_only_name(t_env *temp_env, char *str)
{
	temp_env->name = malloc(ft_strlen(str) + 1);
	if (temp_env->name == NULL)
		exit(errtx_all_free_exit(1, "for temp_env malloc failed"));
	ft_strlcpy(temp_env->name, str, ft_strlen(str) + 1);
	temp_env->value = NULL;
	temp_env->next = NULL;
	find_name_delate(temp_env->name);
	add_env_tolast(temp_env);
}

int	run_export_builtin(t_cmd *cmd)
{
	int		i;
	t_env	*temp_env;

	i = 1;
	if (cmd->args[1] == NULL)
		return (print_env_export());
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '=' || check_valid_identifier(cmd->args[i]) != 0)
			return (write_identifier_error(cmd->args[i]));
		temp_env = malloc(sizeof(t_env) * 1);
		if (temp_env == NULL)
			exit(errtx_all_free_exit(1, "for temp_env malloc failed"));
		if (ft_strchr(cmd->args[i], '=') != NULL)
		{
			temp_env = find_and_split(cmd->args[i], '=', temp_env);
			find_name_delate(temp_env->name);
			add_env_tolast(temp_env);
		}
		else
			env_only_name(temp_env, cmd->args[i]);
		i++;
	}
	return (0);
}
