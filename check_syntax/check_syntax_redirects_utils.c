#include "check_syntax.h"

void skip_alphas_spaces_pipes(t_source *src)
{
	int	 i = 0; // just or testing

	char c;

	if (src == NULL || src->inputline == NULL)
		return;

	//printf(BLU" Start skip alphas ..."RES);
	
	while (((c = peek_next_char(src)) != ENDOFLINE)
		&& (is_allowed_char(c) || ft_isspace(c) || c == '|')
		&& c != '<' && c != '>')
		// get_next_char(src);		// probably can just be currpos++
	{
		//printf(BLU"  %c, i%d"RES, c, i);
		
		if (c == '"' || c == '\'') //	 new jaka, skip all chars between quotes
		{

			//printf(GRN"    skip till pipe, pos %ld, [%c]\n"RES, src->currpos, src->inputline[src->currpos]);
			// if (src->inputline[src->currpos + 1] == c) // added to handle "" // NO THIS IS MESSING: echo "aaa < < bbb"
			// {
			// 	src->currpos += 1;
			// 	return;
			// }



			//printf(GRN" %c"RES, c);
			src->currpos += 2;
			// while (src->inputline[src->currpos] != '"')
			while (src->inputline[src->currpos] != c)
			{
				printf(BLU" %c"RES, c);			// THIS REPEATEDLY PRINTS  " " " " ...
				src->currpos++;
			}
			//printf(GRN" +%c"RES, c);
			src->currpos++;
			//exit (0);
			//c = src->inputline[src->currpos];
		}
		i++;
		// if (i > 20)
		// 	exit(0);
		printf(RED" %c"RES, c);

		//printf(BLU"\n"RES);

		src->currpos++;
	}
	printf(YEL" %d[%c]"RES, c, c);			// THIS IS -1, STRANGE CHARACTER
	printf(YEL"\n"RES);
}



int	is_space_alpha_or_pipe(t_source *src, int *c)
{
	skip_alphas_spaces_pipes(src);
	if (src->inputline[src->currpos + 1] == '<'
		|| src->inputline[src->currpos + 1] == '>')
	{
		src->currpos++;
		*c = src->inputline[src->currpos];
	}
	else if (src->inputline[src->currpos + 1] == '\0')
		return (1);
	return (0);
}


int	check_char_after_space(t_source *src, int *c)
{
	src->currpos += 2;
	skip_white_spaces(src);
	if (src->inputline[src->currpos + 1] == '<'
		|| src->inputline[src->currpos + 1] == '>')	//  <<  < 
		return (1);
	else if (ft_isalpha(src->inputline[src->currpos + 1]))	//  <<  abc
	{
		src->currpos +=2;
		*c = src->inputline[src->currpos];
	}
	return (0);
}