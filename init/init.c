#include "../minishell.h"

sig_atomic_t	g_flag;

void	errtext_exit(char *text)
{
	perror(text);
	exit(ERROR_RETURN);
}
void	free_info(t_infos *infos)
{
	t_env	*env;
	t_env	*temp_env;

	env = infos->start_env;
	while (env->next)
	{
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		temp_env = env;
		env = env->next;
		free(temp_env);
	}
	if (env->name)
		free(env->name);
	if (env->value)
		free(env->value);
	free(env);
}

void	err_free_env_exit(t_infos *infos, char *text)
{
	perror(text);
	free_info(infos);
	exit(ERROR_RETURN);
}

void	handle_sigint(int num)
{
	(void) num;
	g_flag = 1;
	write(STDIN_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

t_env	*last_env(t_env *start_env)
{
	t_env	*last;

	last = start_env;
	while (last->next != NULL)
		last = last->next;
	return (last);
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

void	print_env(t_infos *info)
{
	t_env	*env;

	env = info->start_env;
	while(env->next)
	{
		printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	printf("%s=%s\n", env->name, env->value);
}

void	ms_init(t_infos *info, char *envp[])
{
	int	rc;
	int i = 0;

	rc = tcgetattr(0, &info->termios_save);
	if (rc)
		errtext_exit("get termios failed\n");
	info->termios_new = info->termios_save;
	info->termios_new.c_iflag &= BRKINT;
	info->termios_new.c_lflag &= ~(ECHOCTL);
	info->termios_new.c_cc[VQUIT] = 0;
	info->termios_new.c_cc[VEOF] = 4;
	info->termios_new.c_cc[VINTR] = 3;
	rc = tcsetattr(0, 0, &info->termios_new);
	if (rc)
		errtext_exit("set termios failed\n");
	info->start_env = get_env(info, envp);
	signal(SIGINT, handle_sigint);
	ft_memccpy(info->prompt, "minishell > ", '\0', 13);
	while(envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	printf("-- -- -- --\n");
	print_env(info);
}

char	*name_expand(t_infos *info, char *tx)
{
	char	*newtx;
	t_env	*env;

	newtx = tx;
	env = info->start_env; 
	while (env)
	{
		if (ft_strncmp(tx, env->name, ft_strlen(tx)) == 0)
		{
			newtx = malloc(ft_strlen(env->value) + 1);
			if (newtx == NULL)
				errtext_exit("check expand malloc failed");
			newtx = ft_memcpy(newtx, env->value, ft_strlen(env->value));
			newtx[ft_strlen(env->value)] = '\0';
			free(tx);
			return (newtx);
		}
		env = env->next;
	}
	return (newtx);
}

char	*check_expand(t_infos *info, char *tx)
{
	int		i;
	char	**temp;
	char	*expanded;
	char	*old;

	i = 0;
	expanded = NULL;
	old = NULL;
	if (ft_strchr(tx, '$') == NULL)
		return (tx);
	temp = ft_split(tx, '$');
	if (temp == NULL)
		errtext_exit("check expand split failed");
	while(temp[i] != NULL)
	{
		printf("before expanded : %s\n", temp[i]);
		temp[i] = name_expand(info, temp[i]);
		printf("expanded : %s\n", temp[i]);
		i++;
	}
	i = 0;
	while(temp[i] != NULL)
	{
		printf("hello\n");
		expanded = ft_strjoin_free(expanded, temp[i]);
		if (expanded == NULL)
			errtext_exit("expand text failed");
		i++;
	}
	// free(temp);
	printf("end expanded : %s\n", expanded);
	return (expanded);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_infos	info;
	char	*line;

	line = NULL;
	(void) argc;
	(void) argv;
	ms_init(&info, envp);
	line = readline(info.prompt);
	while (line)
	{
		line = check_expand(&info, line);
		printf("%s\n", line);
		if (strlen(line) > 0)
			add_history(line);
		// here parsing and make a linkedlist of t_cmd
		// after making t_cmd list, fork and execute
		free(line);
		line = readline(info.prompt);
	}
	printf("end!\n");
	rl_clear_history();
	free_info(&info);
	return (0);
}
