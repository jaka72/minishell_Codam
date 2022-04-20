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

void	check_option_print_word(t_cmd *cmd, int *flagw, int *flagl, int *i)
{
	int	j;
	char *word;

	word = cmd->args[*i];

	//if (cmd->args[*i][0] == '"' && cmd->args[*i][1] == '"')
	if (word[0] == '"' && word[1] == '"')
	{
		//printf(MAG"found quote\n"RES);
		// skip until it contains new "" or alphas, until '\0'
		j = 0;
		while (word[j] != '\0')
		{
			if (word[j] != '"')
				printf("%c", word[j]);

			j++;
		}
		printf(" ");  // BUT NOT IF IT IS THE LAST WORD
	}

	else if (word[0] != '-' && word[1] != 'n')		// 	is not -n
	{
		//printf(YEL"working"RES);
		j = 0;
		while (word[j] != '\0')
		{
			if (word[j] != '"')
				printf("%c", word[j]);
			j++;
		}

		if (cmd->args[*i + 1] != NULL)
			if (cmd->args[*i + 1][0] != '-' && cmd->args[*i + 1][1] != 'n')
				printf(" ");
		if (*flagw == 0)
			*flagw = 1;
	}

	else														// is -n
	{
		check_n_option(cmd->args[*i]);
		if (cmd->args[*i + 1] != NULL)
			if (cmd->args[*i + 1][0] != '-' && cmd->args[*i + 1][1] != 'n')
				if (*flagw != 0)
					printf(" ");
		*flagl = 1;
	}
}

// - STILL NEEDS TO HANDLE OPTION -n IF IT IS IN QUOTES
// - HANDLE BOTH ""  INSIDE '' AND VICE VERSA

int	run_echo_builtin(t_cmd *cmd)
{
	printf(RED"Start builtin echo()\n"RES);
	printf(RED"PATH_MAX %d\n"RES, PATH_MAX);
	//printf(RED"MAXPATHLEN %d\n"RES, MAXPATHLEN);

	int	i;
	int	flag_newline;
	int	flag_first_word;

	flag_first_word = 0;
	flag_newline = 0;
	i = 1;
	while (cmd->args[i])
	{
		//printf(YEL"loop [%s]\n"RES, cmd->args[i]);
		check_option_print_word(cmd, &flag_first_word, &flag_newline, &i);
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



aaa bbb

*/
