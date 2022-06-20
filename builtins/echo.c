#include "../minishell.h"

static	int	check_n_option(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 1;
	j = 1;
	while (cmd->args[j] != NULL)
	{
		if (cmd->args[j][0] == '-')
		{
			i = 1;
			while (cmd->args[j][i])
			{
				if (cmd->args[j][i] != 'n')
					return (j);
			}
		}
		else
			return (j);
		j++;
	}
	return (j);
}

static	void	check_line_and_print(t_cmd *cmd, int *flagfw, int *i)
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
	if (*flagfw == 0)
		*flagfw = 1;
}

int	run_echo_builtin(t_cmd *cmd)
{
	int	i;
	int	flag_newline;
	int	flag_first_word;

	if (count_elems(cmd->args) == 1)
	{
		write(1, "\n", 1);
		return (0);
	}
	flag_first_word = 0;
	flag_newline = 0;
	if (cmd->args[1][0] == '-')
		flag_newline = check_n_option(cmd);
	i = 1;
	if (flag_newline > 0)
		i = flag_newline;
	while (cmd->args[i])
	{
		check_line_and_print(cmd, &flag_first_word, &i);
		i++;
	}
	if (flag_newline <= 1)
		write(1, "\n", 1);
	return (0);
}
// - STILL NEEDS TO HANDLE OPTION -n IF IT IS IN QUOTES
// - HANDLE BOTH ""  INSIDE '' AND VICE VERSA
