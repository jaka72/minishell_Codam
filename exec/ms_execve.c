/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_execve.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 09:48:54 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

static void	print_error_msg(char *s1, char *s2, char *s3)
{
	write(2, s1, ft_strlen(s1));
	write(2, s2, ft_strlen(s2));
	write(2, s3, ft_strlen(s3));
}

static int	cmd_no_slash(char *path, char **args, char **envs)
{
	int	ret;

	if (args == NULL || args[0] == NULL)
		return (0);
	if (path == NULL)
	{
		print_error_msg("minishell: ", args[0], ": Command not found\n");
		exit(err_all_free_exit(127));
	}
	else
	{
		ret = access(path, X_OK);
		if (ret != 0)
		{
			print_error_msg("minishell: ", args[0], ": Permission denied\n");
			exit(err_all_free_exit(126));
		}
		else
		{
			execve(path, args, envs);
			print_error_msg("minishell: ", args[0], ": Command not found\n");
			exit(err_all_free_exit(127));
		}
	}
	return (1);
}

static int	cmd_with_slash(char *path, char **args, char **envs)
{
	int	ret;

	ret = access(path, X_OK);
	if (ret != 0)
	{
		if (errno == 2)
		{
			print_error_msg("minishell: ", path, ": No such file or folder\n");
			exit(err_all_free_exit(127));
		}
		else if (errno == 13)
		{
			print_error_msg("minishell: ", path, ": Permission denied\n");
			exit(err_all_free_exit(126));
		}
	}
	else
	{
		execve(path, args, envs);
		print_error_msg("minishell: ", args[0], ": is a directory\n");
	}
	return (1);
}

static int	cmd_is_custom(char **args, char **envs)
{
	int	ret;

	ret = access(args[0], X_OK);
	if (ret != 0)
	{
		if (errno == 2)
		{
			print_error_msg("minishell: ", args[0],
				": No such file or folder\n");
			exit(err_all_free_exit(127));
		}
		else if (errno == 13)
		{
			print_error_msg("minishell: ", args[0], ": Permission denied\n");
			exit(err_all_free_exit(126));
		}
	}
	else
	{
		ret = execve(args[0], args, envs);
		print_error_msg("minishell: ", args[0], ": is a directory\n");
		exit(err_all_free_exit(126));
	}
	return (1);
}

int	ms_execve(t_cmd *str)
{
	char	**envs;
	char	*path;
	int		ret;

	envs = get_env_array();
	if (envs == NULL)
		return (-1);
	if (str->args == NULL)
		exit(err_all_free_exit(0));
	path = ft_findshell_path(str->args[0], envs);
	if (ft_strchr(str->args[0], '/') == 0)
		ret = cmd_no_slash(path, str->args, envs);
	else
	{
		if (path != NULL)
			ret = cmd_with_slash(path, str->args, envs);
		else
			ret = cmd_is_custom(str->args, envs);
	}
	exit(err_all_free_exit(ret));
}
