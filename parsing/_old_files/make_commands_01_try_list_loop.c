#include "../main.h"
#include "make_commands.h"


void free_all(t_source *src)
{
	if (src->tok_buf != NULL)
	{
		free(src->tok_buf);
		src->tok_buf = NULL;
	}
}



int	get_length_of_word(t_source *src)
{
	int	len;

	len = 0;
	while (src->inputline[src->currpos] != '|' && 
			src->inputline[src->currpos] != '<' &&
			src->inputline[src->currpos] != '>' &&
			src->inputline[src->currpos] != '\0' &&
			!isspace(src->inputline[src->currpos]))
	{
		//printf(LGRE"     len%d pos%ld[%c]\n", len, src->currpos, src->inputline[src->currpos]); // store rdr_in
		len++;
		src->currpos++;
	}
	src->currpos -= 1;
	return (len);
}



int	store_into_command_arr(t_source *src, t_cmd *cmd)
{
	int	len;
	int	start;
	
	// ALLOCATE CORRECT NR OF PLACES IN **args
	if (cmd->count_args == 1)	// just started, still empty; Create space for 1st and NULL 
		cmd->args = malloc(sizeof(char*) * 2);
	else
		cmd->args = realloc(cmd->args, sizeof(char*) * (cmd->count_args + 1)); // +1 for NULL

	len = get_length_of_word(src);
		// check return
	
	// CALCULATE CORRECT CURR POSITION TO START COPYING THE WORD INTO STRING args[i]
	start = src->currpos - len + 1;
	//printf(GRN"len: %d, start: %d\n"RES, len, start);

	// ALLOCATE SPACE FOR A WORD
	cmd->args[cmd->count_args - 1] = malloc(sizeof(char) * (len + 1));
		// check return

	// COPY DIRECTLY FROM INPUT LINE, AT CORRECT CURSOR ///////////// HERE
	ft_strlcpy(cmd->args[cmd->count_args - 1], &src->inputline[start], len + 1);

	//printf(GRN"Last word of command: [%s]\n"RES, cmd->args[cmd->count_args - 1]);

	cmd->args[cmd->count_args] = NULL;  // TERMINATE THE ARRAY
	//printf(GRN"Word: [%s]\n"RES, word);

	return (0);
}


typedef struct s_temporary_array
{
	int		arrow;
	int		len;
	int		start;
	char	**rdr_array;
	int		count;
} t_tmp;



void	choose_correct_array(t_source *src, t_cmd *cmd, t_tmp *t)
{
	// CHOOSE IN / OUT ARRAY. OR HEREDOC ARRAY
	if (t->arrow == '<' && src->inputline[src->currpos + 1] != '<')		// INFILE <
	{
		t->rdr_array = cmd->infile;
		cmd->count_infiles++;
		t->count = cmd->count_infiles;
		cmd->fd_in = -2;
	}
	else if (t->arrow == '<' && src->inputline[src->currpos + 1] == '<') // HEREDOC >>
	{
		t->rdr_array = cmd->heredoc;
		cmd->count_heredocs++;
		t->count = cmd->count_heredocs;
		cmd->fd_in = -3;
		src->currpos++;
	}
	else if (t->arrow == '>') 											// OUTFILE >
	{
		t->rdr_array = cmd->outfile;
		cmd->count_outfiles++;
		t->count = cmd->count_outfiles;
		cmd->fd_out = -2;
		if (src->inputline[src->currpos + 1] == '>') 					// APPEND >> 
		{
			src->currpos++;
			cmd->fd_in = -3;	// APPEND MODE
		}
	}
}


int	store_into_redirect_arr(t_source *src, t_cmd *cmd)
{
	t_tmp	t;

	t.count = 0;
	t.rdr_array = NULL;
	t.arrow = src->inputline[src->currpos];  // '<' OR '>'

	// CHOOSE IN / OUT ARRAY. OR HEREDOC ARRAY
	choose_correct_array(src, cmd, &t);

	// ALLOCATE CORRECT NR OF PLACES IN **args
	if (t.count == 1)	// just started, still empty; Create space for 1st and NULL 
		t.rdr_array = malloc(sizeof(char*) * 2);
	else
		t.rdr_array = realloc(t.rdr_array, sizeof(char*) * (t.count + 1)); // +1 for NULL

	src->currpos++;
	skip_white_spaces(src);
	//printf(YEL"    rdr first char %ld[%c]\n"RES, src->currpos, src->inputline[src->currpos]);
	///////////////////

	t.len = get_length_of_word(src);
		// check return
	
	// CALCULATE CORRECT CURR POSITION TO START COPYING THE WORD INTO STRING args[i]
	t.start = src->currpos - t.len + 1;
	//printf(GRN"len: %d, start: %d\n"RES, len, start);

	// ALLOCATE SPACE FOR A WORD
	t.rdr_array[t.count - 1] = malloc(sizeof(char) * (t.len + 1));
		// check return

	// COPY DIRECTLY FROM INPUT LINE, AT CORRECT CURSOR ///////////// HERE
	ft_strlcpy(t.rdr_array[t.count - 1], &src->inputline[t.start], t.len + 1);

	if (t.arrow == '<' && cmd->fd_in == -2)		// INFILE <
		cmd->infile = &t.rdr_array[0];
	else if (t.arrow == '<' && cmd->fd_in == -3)	// HEREDOC <<
		cmd->heredoc = &t.rdr_array[0];
	else if (t.arrow == '>')						// OUTFILE > OR >>
		cmd->outfile = &t.rdr_array[0];


	t.rdr_array[t.count] = NULL;  // TERMINATE THE ARRAY
	return (0);
}


void	init_values(t_source *src, t_cmd *cmd)
{
	src->temp = 1;
	// src->currpos = -1;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc = NULL;
	cmd->count_args = 0;
	cmd->count_infiles = 0;
	cmd->count_outfiles = 0;
}



int	select_and_store_words(t_source *src, t_cmd *cmd)
{
	int	ch;

	//printf("select\n");
	init_values(src, cmd);


	while (1)
	{
		skip_white_spaces(src);
		src->currpos++;
		ch = src->inputline[src->currpos];
		//printf(LBLU"Loop start: pos%ld[%c]\n"RES, src->currpos, src->inputline[src->currpos]);
		if (ch == '<' || ch == '>')
		{	
			//printf("  found rdr\n"); // store rdr_in
			store_into_redirect_arr(src, cmd);
		}
		else if (isalpha(ch) || ch == '-') // MUST BE is_allowed_char()
		{
			//printf(BLU"  found alpha %ld[%c]\n"RES, src->currpos, ch); // store rdr_in
			cmd->count_args++;
			store_into_command_arr(src, cmd); 	// store command name
		}
		else if (ch == '|')
		{
			//printf(MAG"Found pipe: Start new cycle.\n"RES); // store rdr_in
			break ;
		}
		else if (ch == '\0')
		{
			// printf(MAG"Found End of line: return 1\n"RES); // store rdr_in
			return (1);
		}
		//printf("Loop end: pos%ld[%d]\n", src->currpos, src->inputline[src->currpos]);
		//exit(0);
	}
	return (0);
}



//////////////////////////////////////////////////////////////////


int	main(int argc, char **argv)
{
	int			i;
	int			ret;
	t_source	src;
	t_cmd		*cmd;

	if (argc == 2)
	{
		src.inputline = argv[1];
		src.inputline_size = strlen(src.inputline);
		printf("line len: %ld\n", src.inputline_size);
	}
	else
	{
		printf("\nPls provide the line!\n");
		return (0);
	}

	src.currpos = -1;


	i = 0;
	cmd = NULL;
	while (1)
	{
		cmd = malloc(sizeof(t_cmd));
		ret = select_and_store_words(&src, cmd);
		printf(LBLU"\nCOMMAND %d\n"RES, i);
		print_command_info(cmd);
		if (ret == 1)
		{
			cmd->next = NULL;
			printf(BLU"Found End of line: return 1\n"RES); // store rdr_in
			break ;
		}
		printf(BLU"Found pipe: Start new cycle.\n"RES); // store rdr_in
		cmd->next = cmd;
		cmd->next = NULL;
		i++;
	}

	// free(cmd.args);
	// free(cmd.infile);
	// free(cmd.outfile);
	// free(cmd.heredoc);
}
