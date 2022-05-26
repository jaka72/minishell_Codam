#include "builtins.h"


int	check_n_option(char *str)
{
	int	i;

	i = 1;		// '-n' skip dash, start at first 'n'
	while (str[i])
	{
		if (str[i] != 'n')
		{
			//printf(YEL"   No option -n\n"RES);
			return (0);
		}
		i++;
	}
	//printf(YEL"    Option -n is ON (ignore new line)\n"RES);
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
		//if (word[j] != '"')
			//printf("%c", word[j]);
			write(1, &word[j], 1);
		j++;
	}
	if (cmd->args[*i + 1] != NULL)
	{
		//printf(" ");
		write(1, " ", 1);
	}
	if (*flagw == 0)
		*flagw = 1;
}


// - STILL NEEDS TO HANDLE OPTION -n IF IT IS IN QUOTES
// - HANDLE BOTH ""  INSIDE '' AND VICE VERSA
int	run_echo_builtin(t_cmd *cmd /*, t_infos *info */)
{
	//printf(RED"Start echo %d\n"RES, 1);
	int	i;
	int	flag_newline;
	int	flag_first_word;

	if (cmd->count_args == 1)
	{
		write(1, "\n", 1);
		return (0);
	}
	flag_first_word = 0;
	// CHECK FIRST ARG IF IT IS -n


	// ADDED
	flag_newline = 0;
	if (cmd->args[1][0] == '-')
		flag_newline = check_n_option(cmd->args[1]);
	//printf(YEL"    flag_newline: %d: \n"RES, flag_newline);
	i = 1;
	if (flag_newline == 1)
		i = 2;
	printf(YEL"Echo, arg[1]: [%s], i=%d, flag=%d\n"RES, cmd->args[1], i, flag_newline);
	while (cmd->args[i])
	{
		//printf(YEL"loop [%s], i=%d\n"RES, cmd->args[i], i);
		check_line_and_print(cmd, &flag_first_word, /*&flag_newline, */ &i /*, info */);
		i++;
	}
	if (flag_newline == 0)
		write(1, "\n", 1);
	return (0);
}
