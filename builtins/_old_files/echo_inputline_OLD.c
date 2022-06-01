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

void	check_word(t_source *src, int *flagw, int *flagl, int *i)
{
	*i = *i +1;
	*flagw = *flagw + 1;

	//printf(YEL"       a) pos %ld\n"RES, src->currpos);

	char *line = &src->inputline[src->currpos];
	if (line[0] == '"' && line[1] == '"')		// FOUND ""
	{
		//printf(YEL"FOUND 2 double quotes\n"RES);
		if (ft_isspace(line[2]))
		{
			src->currpos += 2;
			//printf(YEL"    Found space after double, pos %ld\n"RES, src->currpos);
			skip_white_spaces(src);
			//printf(YEL"    After skip spaces, pos %ld\n"RES, src->currpos);
			if (src->inputline[src->currpos + 1] != '\0')
				printf("e");
			else
				printf(YEL"Found EOL after 2 double quotes\n"RES);
		}
	}
	else if (line[0] == '"' && line[1] == '-')		// FOUND "-
	{
		if (line[2] == 'n')							// FOUND "-n
		{
			src->currpos += 2;
			while (src->inputline[src->currpos] == 'n')
			{
				src->currpos++;
			}
			if (src->inputline[src->currpos] == '"')		// closing quote: "-n"  OR  "-nnnnn"
				*flagl = 1;
		}
	}
	else if (line[0] == '"' && line[1] != '-')
	{
		//printf(YEL"Found allowed after open quote \n"RES);

		src->currpos += 1;
		while (src->inputline[src->currpos] != '"')
		{
			printf(MAG"%c"RES, src->inputline[src->currpos]);
			src->currpos++;
		}
		skip_white_spaces(src);
		if (src->inputline[src->currpos + 1] != '\0')
			printf("a"); // PUT SPACE AFTER "abc"
	}
	else if (is_allowed_char(line[0]))
	{
		while (src->inputline[src->currpos] != '"' && src->inputline[src->currpos] != '\0'
			&& src->inputline[src->currpos] != '$' && src->inputline[src->currpos] != ' ')
		{
			printf(BLU"%c"RES, src->inputline[src->currpos]);
			src->currpos++;
		}
		skip_white_spaces(src);
		if (src->inputline[src->currpos + 1] != '\0')
			printf("n"); // PUT SPACE AFTER "abc"
	}


	// if (cmd->args[*i][0] != '-' && cmd->args[*i][1] != 'n')		// if it is not -n
	// {
	// 	printf("%s", cmd->args[*i]);
	// 	if (cmd->args[*i + 1] != NULL)
	// 		if (cmd->args[*i + 1][0] != '-' && cmd->args[*i + 1][1] != 'n')
	// 			printf(" ");
	// 	if (*flagw == 0)
	// 		*flagw = 1;
	// }
	// else
	// {
	// 	check_n_option(cmd->args[*i]);
	// 	if (cmd->args[*i + 1] != NULL)
	// 		if (cmd->args[*i + 1][0] != '-' && cmd->args[*i + 1][1] != 'n')
	// 			if (*flagw != 0)
	// 				printf(" ");
	// 	*flagl = 1;
	// }
}	

int	echo_builtin(t_source *src)
{
	printf(RED"Start builtin echo()\n"RES);

	int	i;
	int	flag_newline;
	int	flag_first_word;

	flag_first_word = 0;
	flag_newline = 0;
	i = 1;
	src->currpos = 0;
	skip_white_spaces(src);
	src->currpos += 5;
	while (src->currpos < src->inputline_size)
	{
		//printf(RED" start loop [%c] \n"RES, src->inputline[src->currpos]);
		check_word(src, &flag_first_word, &flag_newline, &i);
		src->currpos++;
		i++;
		//printf(RED" end loop %ld[%c] \n"RES, src->currpos, src->inputline[src->currpos]);
	}
	if (flag_newline == 0)
		printf("\n");
	return (0);
}


/*
If invalid char is inside " ", do I still print it?
		echo "aaa\bbb"
		cat "infile"


CPP MODULE 05
Header files, .h or hpp ?
In the header, there is a definition of a class
Forbiden in header: implementation of function
Forbidden double inclusion of headers
Leaks

Ex00
Instantiate a burocrat ?
Throw exception for invalid grade (toohigh, toolow)
For both atributtes ???? you must provide getters (getname, getgrade) ???

The getters must throw the same exceptions as the constructor ???


echo "aaa " "" "bbb"
aaa   bbb
aaa   bbb

echo "aaa " " " "bbb"
aaa    bbb


a = 1 space after alpha   "abc" 
e = 1 space after empty    "" 
n = 1 space after normal   abc 


*/
