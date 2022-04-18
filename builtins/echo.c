#include "builtins.h"

int	check_n_option(char *str)
{
	int	i;

	i = 1;		// '-n' skip dash, start at first 'n'
	while (str[i])
	{
		if (str[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

void	print_word_check_option(t_cmd *cmd, int *flagw, int *flagl, int *i)
{
	if (cmd->args[*i][0] != '-' && cmd->args[*i][1] != 'n')		// if it is not -n
	{
		printf("%s", cmd->args[*i]);
		if (cmd->args[*i + 1] != NULL)
			if (cmd->args[*i + 1][0] != '-' && cmd->args[*i + 1][1] != 'n')
				printf(" ");
		if (*flagw == 0)
			*flagw = 1;
	}
	else
	{
		check_n_option(cmd->args[*i]);
		if (cmd->args[*i + 1] != NULL)
			if (cmd->args[*i + 1][0] != '-' && cmd->args[*i + 1][1] != 'n')
				if (*flagw != 0)
					printf(" ");
		*flagl = 1;
	}
}	

int	echo_builtin(t_cmd *cmd)
{
	int	i;
	int	flag_newline;
	int	flag_first_word;

	flag_first_word = 0;
	flag_newline = 0;
	i = 1;
	while (cmd->args[i])
	{
		//printf(YEL"loop [%s]\n"RES, cmd->args[i]);
		print_word_check_option(cmd, &flag_first_word, &flag_newline, &i);
		i++;
	}
	if (flag_newline == 0)
		printf("\n");
	return (0);
}


/*
If invalid char is inside " ", do I still print it?
		echo "aaa\bbb"
		cat "infile"

*/