/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 12:41:35 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/02 17:28:50 by jmurovec      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	check_n_option(char *str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void	check_line_and_print(t_cmd *cmd, int *flagw, int *i)
{
	int		j;
	char	*word;

	word = cmd->args[*i];
	j = 0;
	while (word[j] != '\0')
	{
		write(1, &word[j], 1);
		j++;
	}
	if (cmd->args[*i + 1] != NULL)
	{
		write(1, " ", 1);
	}
	if (*flagw == 0)
		*flagw = 1;
}

int	count_args(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
		i++;
	return (i);
}



// - STILL NEEDS TO HANDLE OPTION -n IF IT IS IN QUOTES
// - HANDLE BOTH ""  INSIDE '' AND VICE VERSA
// printf(YEL"Echo, arg[1]: [%s], i=%d, flag=%d\n"RES,
//					cmd->args[1], i, flag_newline);
//printf(YEL"loop [%s], i=%d\n"RES, cmd->args[i], i);
int	run_echo_builtin(t_cmd *cmd)
{
	int	i;
	int	flag_newline;
	int	flag_first_word;

	int	nr_args;
	nr_args = count_args(cmd);

	//printf("nr args: %d\n", nr_args);

	// if (cmd->count_args == 1)
	if (nr_args == 1)
	{
		write(1, "\n", 1);
		return (0);
	}
	flag_first_word = 0;
	flag_newline = 0;
	if (cmd->args[1][0] == '-')
		flag_newline = check_n_option(cmd->args[1]);
	i = 1;
	if (flag_newline == 1)
		i = 2;
	while (cmd->args[i])
	{
		check_line_and_print(cmd, &flag_first_word, &i);
		i++;
	}
	if (flag_newline == 0)
		write(1, "\n", 1);
	return (0);
}
