#include "builtins.h"

int	check_n_option(char *str)
{
	int	i;

	i = 0;		// '-n' skip dash, start at first 'n'
	if (str == NULL || str[0] != '-')
		return (0);
	else
	{
		printf(RED"    Found - \n"RES);

		while (str[i])
		{
			if (str[i] != 'n')
			{
				return (0);
			}
			i++;
		}
	}
	return (1);
}


void	check_line_and_print(t_cmd *cmd, int *flagw, int *i)
{
	int	j;
	char *word;

	word = cmd->args[*i];
	j = 0;
	while (word[j] != '\0')
	{
		printf("%c", word[j]);
		j++;
	}

	if (cmd->args[*i + 1] != NULL)
		printf(" ");
	if (*flagw == 0)
		*flagw = 1;
}


int	run_echo_builtin(t_cmd *cmd)
{

	//printf(RED"Start echo \n"RES);
	int	i;
	int	flag_newline;
	int	flag_first_word;

	flag_first_word = 0;
	
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
		printf("\n");

	return (0);
}
