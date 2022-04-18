#include "../minishell.h"

sig_atomic_t	g_flag;

void	errtext_exit(char *text)
{
	perror(text);
	exit(ERROR_RETURN);
}

void	free_envlist(t_infos *infos)
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
	free_envlist(infos);
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

void	print_env(t_infos *info) // function to print env (not necessary)
{
	t_env	*env;

	env = info->start_env;
	while (env->next)
	{
		printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	printf("%s=%s\n", env->name, env->value);
}

void	ms_init(t_infos *info, char *envp[])
{
	int	rc;
	int i;

	i = 0;
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
	// while(envp[i])
	// {
	// 	printf("%s\n", envp[i]);
	// 	i++;
	// }
	// printf("-- -- -- --\n");
	// print_env(info);
}

char	*name_expand(t_infos *info, char *tx)
{
	char	*newtx;
	t_env	*env;

	newtx = NULL;
	env = info->start_env;
	while (env)
	{
		if (ft_strncmp(tx, env->name, ft_strlen(tx + 1)) == 0
			&& ft_strlen(tx) == ft_strlen(env->name))
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

	i = 0;
	if (ft_strchr(tx, '$') == NULL || (tx[0] == '$' && tx[1] == '\0'))
		return (tx);
	expanded = malloc(1);
	if (expanded == NULL)
		err_free_env_exit(info, "check expand malloc failed");
	expanded[0] = '\0';
	temp = ft_split(tx, '$');
	if (temp == NULL)
		err_free_env_exit(info, "check expand split failed");
	free(tx);
	while (temp[i] != NULL)
	{
		temp[i] = name_expand(info, temp[i]);
		i++;
	}
	i = 0;
	while (temp[i] != NULL)
	{
		expanded = ft_strjoin_free(expanded, temp[i]);
		if (expanded == NULL)
			err_free_env_exit(info, "connect expanded text failed");
		i++;
	}
	free(temp);
	return (expanded);
}

char	*make_malloc_str(char *text)
{
	int		i;
	char	*str;

	if (text == NULL)
		return (NULL);
	i = ft_strlen(text);
	str = malloc(i + 1);
	if (str == NULL)
		errtext_exit("malloc failed\n");
	str = ft_memcpy(str, text, i);
	str[i] = '\0';
	return (str);
}

t_cmd	*make_dammy1(t_infos *info, t_cmd *strdammy)
{
	strdammy = malloc(sizeof(t_cmd) * 1);
	if (strdammy == NULL)
		errtext_exit("making dammy failed\n");
	strdammy->start_env = info->start_env;
	strdammy->args = NULL;
	strdammy->fd_in = -2;
	strdammy->fd_out = 1;
	strdammy->infile = make_malloc_str("infile");
	strdammy->outfile = make_malloc_str(NULL);
	strdammy->next = NULL;

	return (strdammy);
}

t_cmd	*make_dammy2(t_infos *info, t_cmd *strdammy)
{
	strdammy = malloc(sizeof(t_cmd) * 1);
	if (strdammy == NULL)
		errtext_exit("making dammy failed\n");
	strdammy->start_env = info->start_env;
	strdammy->args = NULL;
	strdammy->fd_in = 0;
	strdammy->fd_out = 1;
	strdammy->infile = make_malloc_str(NULL);
	strdammy->outfile = make_malloc_str(NULL);
	strdammy->next = NULL;

	return (strdammy);
}

t_cmd	*manage_in_out(t_cmd *str)
{
	int	i;

	i = 0;
	if (str->fd_in == -2)
	{
		if (access(str->infile, F_OK) != 0)
			errtext_exit("infile not exist\n");
		if (access(str->infile, F_OK) == 0 && access(str->infile, R_OK) < 0)
			errtext_exit("can't access infile\n");
		str->fd_in = open(str->infile, O_RDONLY);	
		if (str->fd_in < 0)
			errtext_exit("file open failed\n");
	}
	if (str->fd_in == -3)
	{
		printf("here doc function!\n");
	}
	if (str->fd_out == -2)
	{
		if (access(str->outfile, F_OK) == 0 && access(str->outfile, W_OK) < 0)
			errtext_exit("outfile exist but not accessible\n");
		str->fd_out = open(str->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (str->fd_out < 0)
			errtext_exit("output file open failed\n");
	}
	if (str->fd_out == -3)
	{
		if (access(str->outfile, F_OK) == 0 && access(str->outfile, W_OK) < 0)
			errtext_exit("outfile exist but not accessible\n");
		str->fd_out = open(str->outfile, O_WRONLY | O_CREAT, 0666);
		if (str->fd_out < 0)
			errtext_exit("output file open failed\n");
	}
	return (str);
}

int	run_cmd(t_infos *info, t_cmd *str)
{
	t_cmd	*current;
	pid_t	pid;

	current = str;
	if (current->next == NULL) //if no pipe
	{
		//check if that is the builtin
		if(current->fd_in != 0)
			dup2(current->fd_in, 0);
		if (current->fd_out != 1)
			dup2(current->fd_out, 1);
		pid = fork();
		if (pid == 0)
		{
			
		}
	}

	return(0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_infos	info;
	char	*line;
	t_cmd	*str1 = NULL;
	t_cmd	*str2 = NULL;
	t_cmd	*str_start = NULL;


	(void) argc;
	(void) argv;
	ms_init(&info, envp);
	str1 = make_dammy1(&info, str1);
	str2 = make_dammy2(&info, str2);
	str1->next = str2;
	str_start = str1;
	line = readline(info.prompt);
	while (line)
	{
		line = check_expand(&info, line);
		if (ft_strlen(line) > 0)
		{
			printf("%s\n", line);
			add_history(line);
		}	
		// here parsing and make a linkedlist of t_cmd
		// after making t_cmd list, fork and execute
		if (run_cmd(&info, str_start) < 0)
			errtext_exit("executing failed\n");
		free(line);
		line = readline(info.prompt);
	}
	printf("exit!\n");
	rl_clear_history();
	free_envlist(&info);
	free(line);
	return (0);
}
