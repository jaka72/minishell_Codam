/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 19:26:43 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
				msg_and_exit("for env name malloc failed\n", 1);
			ft_strlcpy(env->name, s, i + 1);
			env->name[i] = '\0';
			env->value = malloc(sizeof(char) * (w - i));
			if (env->value == NULL)
				msg_and_exit("for env value malloc failed\n", 1);
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

static	void	env_only_name(t_env *temp_env, char *str, t_util *st_base)
{
	temp_env->name = malloc(ft_strlen(str) + 1);
	if (temp_env->name == NULL)
		msg_and_exit("for temp_env malloc failed\n", 1);
	ft_strlcpy(temp_env->name, str, ft_strlen(str) + 1);
	temp_env->value = NULL;
	temp_env->next = NULL;
	find_name_delate(temp_env->name, st_base);
	add_env_tolast(temp_env, st_base);
}

int	run_export_builtin(t_cmd *cmd, t_util *st_base)
{
	int		i;
	t_env	*temp_env;

	i = 1;
	if (cmd->args[1] == NULL)
		return (print_env_export(st_base));
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '=' || check_valid_identifier(cmd->args[i]) != 0)
			return (write_identifier_error(cmd->args[i]));
		temp_env = malloc(sizeof(t_env) * 1);
		if (temp_env == NULL)
			msg_and_exit("for temp_env malloc failed\n", 1);
		if (ft_strchr(cmd->args[i], '=') != NULL)
		{
			temp_env = find_and_split(cmd->args[i], '=', temp_env);
			find_name_delate(temp_env->name, st_base);
			add_env_tolast(temp_env, st_base);
		}
		else
			env_only_name(temp_env, cmd->args[i], st_base);
		i++;
	}
	return (0);
}
