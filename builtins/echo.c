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


void	check_line_and_print(t_cmd *cmd, int *flagw, /* int *flagl,*/ int *i  /*, t_infos *info */)
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


// - STILL NEEDS TO HANDLE OPTION -n IF IT IS IN QUOTES
// - HANDLE BOTH ""  INSIDE '' AND VICE VERSA
int	run_echo_builtin(t_cmd *cmd /*, t_infos *info */)
{
	int	i;
	int	flag_newline;
	int	flag_first_word;

	flag_first_word = 0;
	
	// CHECK 1st ARG IF IT IS -n
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


/*
If invalid char is inside " ", do I still print it?
		echo "aaa\bbb"
		cat "infile"
aaa bbb
*/
