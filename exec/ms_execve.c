/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_execve.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/14 09:35:21 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/16 17:20:03 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error_msg(char *s1, char *s2, char *s3)
{
	write(2, s1, ft_strlen(s1));
	write(2, s2, ft_strlen(s2));
	write(2, s3, ft_strlen(s3));
}

// If path is NULL, it is a custom command, not library command.
int	cmd_no_slash(char *path, char **args, char **envs)
{
	int	ret;

	if (path == NULL)	// is custom
	{
		//printf(BLU"       Path is NULL\n");
		print_error_msg("minishell: ", args[0], ": Command not found\n");
		exit(err_all_free_exit(127));
	}
	else				// is library
	{
		ret = access(path, X_OK);
		if (ret != 0)			// cannot execute
		{
			print_error_msg("minishell: ", args[0], ": Permission denied\n");
			exit(err_all_free_exit(126));
		}
		else					// can execute
		{
			execve(path, args, envs);
			//printf(GRN"      Why did not execute??\n"); // what if it still does not execute?
			print_error_msg("minishell: ", args[0], ": Command not found\n");
			exit(err_all_free_exit(127));
		}
	}
	return (1);		// means it did not execute ??
}

int	cmd_with_slash(char *path, char **args, char **envs)
{
	int	ret;

	ret = access(path, X_OK);
	if (ret != 0)
	{
		//printf(BLU"       Access not zero! 0\n");
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
		//printf(BLU"       Access zero, can execute\n");
		execve(path, args, envs);
		//printf(GRN"      Why did not execute??\n");  // what if it still does not execute?
		print_error_msg("minishell: ", args[0], ": is a directory\n");

	}
	return (1);		// means it did not execute ??
}


int	cmd_is_custom(char **args, char **envs)
{
	int	ret;

	ret = access(args[0], X_OK);
	if (ret != 0)
	{
		// printf(BLU"       Access not zero! 0\n");
		if (errno == 2)
		{
			print_error_msg("minishell: ", args[0], ": No such file or folder\n");
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
		//printf(BLU"       Access is zero, should execute\n");
		ret = execve(args[0], args, envs);
		// printf(BLU"       Execve did not execute\n"); // what if it still does not execute?
		// printf(BLU"       ret: %d, errno: %d\n", ret, errno);
		// perror("          perror: ");
		print_error_msg("minishell: ", args[0], ": is a directory\n");
		exit(err_all_free_exit(126));
	}
	return (1);		// means it did not execute ??
}

// Var path will only return valid path, if args[0] does not contain slashes
// If var args[0] has slashes or dot, it will ignore var path, and execute the args[0]
int	ms_execve(t_cmd *str)
{
	char	**envs;
	char	*path;
	int		ret;

	envs = get_env_array();
	if (envs == NULL)
		return (-1);
	path = ft_findshell_path(str->args[0], envs);
	// signal(SIGINT, SIG_DFL);
	// signal(SIGQUIT, SIG_DFL);
	if (ft_strchr(str->args[0], '/') == 0)	// cmd without slashes
	{
		// printf(GRN"No slashes\n");  // what if it still does not execute?
		ret = cmd_no_slash(path, str->args, envs);
	}
	else								// cmd has slashes
	{
		if (path != NULL)		// is library
		{
			// printf(GRN"Has slashes, is library.\n");  // what if it still does not execute?
			ret = cmd_with_slash(path, str->args, envs);
		}
		else		// is custom or library
		{
			printf(GRN"Cmd is custom, path [%s]\n", path);  // what if it still does not execute?
			ret = cmd_is_custom(str->args, envs);
		}
	}
	//printf(YEL"END of mse_execve, ret = %d\n", ret);
	exit(err_all_free_exit(ret));
}
