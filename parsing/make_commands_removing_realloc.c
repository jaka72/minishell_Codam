#include "make_commands.h"


int	realloc_array(char **str, char **temp)
{
	int	i = 0;

	i = 0;
	while (str[i])
	{
		printf(MAG"   word%d: [%s], "RES, i, str[i]);
		temp[i] = ft_strdup(str[i]);
		i++;
	}
	//printf(MAG"\n"RES);
	i = 0;
	// FREE ALL OLD ARGS ELEMENTS
	while (str[i])
	{
		printf(MAG" free%d [%s]"RES, i, str[i]);
		free(str[i]);
		i++;
	}
	printf(MAG"\n"RES);
	free(str);
	str = temp;
	return (0);
}



int	store_into_command_arr(t_source *src, t_cmd *cmd)
{
	//int		i;
	int		len;
	int		start;
	// char	**temp;
	
	//printf(MAG"store into arr\n"RES);
	// ALLOCATE CORRECT NR OF PLACES IN **args
	if (cmd->count_args == 1)	// just started, still empty; Create space for 1st and NULL 
	{
		cmd->args = malloc(sizeof(char*) * 2);
		if (cmd->args == NULL)
			return (1); // HERE MAKE SURE THAT IT EXITS, FREES, GIVES MESSAGE ETC ... !!!!!!!!
	}
	else
	{
		// NEW CODE - REALLOCING THE ARGS ARRAY
		// temp = malloc(sizeof(char*) * (cmd->count_args + 1));
		// if (temp == NULL)
		// 	return (1);

		// NEW CODE
		// realloc_array(cmd->args, temp);

		// OLD CODE
		// cmd->args = realloc(cmd->args, sizeof(char*) * (cmd->count_args + 1)); // +1 for NULL
		cmd->args = ft_add_str(cmd->args);
		// if (cmd->args == NULL) // maybe not necessary, it already checks temp above
		//	return (1); // HERE MAKE SURE THAT IT EXITS, FREES, GIVES MESSAGE ETC ... !!!!!!!!
	}

	len = get_length_of_word(src);
		// check return
	
	// CALCULATE CORRECT CURR POSITION TO START COPYING THE WORD INTO STRING args[i]
	start = src->currpos - len + 1;

	// ALLOCATE SPACE FOR A WORD
	cmd->args[cmd->count_args - 1] = malloc(sizeof(char) * (len + 1));
		// check return

	// COPY DIRECTLY FROM INPUT LINE, AT CORRECT CURSOR ///////////// HERE
	ft_strlcpy(cmd->args[cmd->count_args - 1], &src->inputline[start], len + 1);
	
	//printf(GRN" terminate the array, countargs[%d]\n"RES, cmd->count_args);
	cmd->args[cmd->count_args] = NULL;  // TERMINATE THE ARRAY
	return (0);
}



void	choose_correct_array(t_source *src, t_cmd *cmd, t_tmp *t)
{
	if (t->arrow == '<' && src->inputline[src->currpos + 1] != '<')		// INFILE <
	{
		t->temp_arr = cmd->infile;
		cmd->count_infiles++;
		t->count = cmd->count_infiles;
		cmd->fd_in = -2;
	}
	else if (t->arrow == '<' && src->inputline[src->currpos + 1] == '<') // HEREDOC >>
	{
		t->temp_arr = cmd->heredoc;
		cmd->count_heredocs++;
		t->count = cmd->count_heredocs;
		cmd->fd_in = -3;
		src->currpos++;
	}
	else if (t->arrow == '>') 											// OUTFILE >
	{
		//printf(BLU"is > outfile (-2)\n"RES);
		t->temp_arr = cmd->outfile;
		cmd->count_outfiles++;
		t->count = cmd->count_outfiles;
		cmd->fd_out = -2;
		if (src->inputline[src->currpos + 1] == '>') 					// APPEND >> 
		{
			//printf(BLU"is >> heredoc (-3)\n"RES);
			src->currpos++;
			cmd->fd_out = -3;	// APPEND MODE
		}
	}
}


int	store_into_redirect_arr(t_source *src, t_cmd *cmd)
{
	t_tmp	t;

	t.count = 0;
	t.temp_arr = NULL;
	t.arrow = src->inputline[src->currpos];  // '<' OR '>'

	// CHOOSE IN / OUT ARRAY. OR HEREDOC ARRAY
	printf("checking array src->currpos is %ld\n", src->currpos);
	choose_correct_array(src, cmd, &t);
	// ALLOCATE CORRECT NR OF PLACES IN **args
	if (t.count == 1)	// just started, still empty; Create space for 1st and NULL
	{
		printf(YEL"Malloced rdr_array FIRST\n"RES);
		t.temp_arr = malloc(sizeof(char*) * 2);
		if (t.temp_arr == NULL)
			return (1); // HERE MAKE SURE THAT IT EXITS, FREES, GIVES MESSAGE ETC ... !!!!!!!!
	}
	else
	{
		// NEW CODE
		printf(YEL"Malloced rdr_array OTHER\n"RES);
		realloc_array(cmd->heredoc, t.temp_arr);

		// OLD CODE
		
		// t.temp_arr = realloc(t.temp_arr, sizeof(char*) * (t.count + 1)); // +1 for NULL
	}
	printf(GRN"    pos%ld[%c]\n"RES, src->currpos, src->inputline[src->currpos]);
	skip_white_spaces(src);
	src->currpos++;
	printf("iiiii\n");

	t.len = get_length_of_word(src);
		// check return
	
	// CALCULATE CORRECT CURR POSITION TO START COPYING THE WORD INTO STRING args[i]
	t.start = src->currpos - t.len + 1;

	// ALLOCATE SPACE FOR A WORD
	t.temp_arr[t.count - 1] = malloc(sizeof(char) * (t.len + 1));
		// check return

	// COPY DIRECTLY FROM INPUT LINE, AT CORRECT CURSOR ///////////// HERE
	ft_strlcpy(t.temp_arr[t.count - 1], &src->inputline[t.start], t.len + 1);

	if (t.arrow == '<' && cmd->fd_in == -2)			// INFILE <
		cmd->infile = &t.temp_arr[0];
	else if (t.arrow == '<' && cmd->fd_in == -3)	// HEREDOC <<
		cmd->heredoc = &t.temp_arr[0];
	else if (t.arrow == '>')						// OUTFILE > OR >>
		cmd->outfile = &t.temp_arr[0];
	t.temp_arr[t.count] = NULL;  // TERMINATE THE ARRAY
	return (0);
}




int	select_and_store_words(t_source *src, t_cmd *cmd)
{
	int	ch;

	init_values(cmd);
	while (1)
	{
		skip_white_spaces(src);
		printf("WWWWW\n");
		src->currpos++;
		ch = src->inputline[src->currpos];
		if (ch == '<' || ch == '>')
		{	
			//printf(CYN"    pos%ld[%c]\n"RES, src->currpos, src->inputline[src->currpos]);
			store_into_redirect_arr(src, cmd);
		}
		else if (is_valid_filename_char(ch) && ch != '\0') // MUST BE is_allowed_char()
		{
			//printf(CYN"  found alpha %ld[%c]\n"RES, src->currpos, ch); // store rdr_in
			cmd->count_args++;
			printf("1\n");
			store_into_command_arr(src, cmd); 	// store command name
		}
		else if (ch == '|')
		{
			//printf(MAG"Found pipe: Start new cycle.\n"RES); // store rdr_in
			break ;
		}
		else if (ch == '\0')
		{
			printf(MAG"Found End of line: return 1\n"RES); // store rdr_in
			return (1);
		}
		//printf("Loop end: pos%ld[%d]\n", src->currpos, src->inputline[src->currpos]);
	}
	printf("DDDDD\n");
	return (0);
}



//////////////////////////////////////////////////////////////////


t_cmd	*make_commands(t_source *src /*, t_infos *info */)
{
	int		i;			// just for printing
	int		ret;
	t_cmd	*first_cmd;
	t_cmd	*new_cmd;
	
	printf("!!!!!!\n");	
	if (src->inputline_size == 0)
		return (0);


	src->currpos = -1;
	first_cmd = NULL;
	i = 0;
	while (1)
	{
		new_cmd = malloc(sizeof(t_cmd));
		ret = select_and_store_words(src, new_cmd);
		printf("\n"BLU"COMMAND %d\n"RES, i);
		// print_command_info(new_cmd);

		if (ret == 1)
		{
			ft_lstadd_back(&first_cmd, new_cmd);
			//printf(BLU"Found End of line: return 1\n"RES); // store rdr_in
			break ;
		}
		printf(BLU"Found pipe: Start new cycle.\n"RES); // store rdr_in
		ft_lstadd_back(&first_cmd, new_cmd);
		i++;
	}

	// printf(MAG"From make_commands\n"RES);
	// t_env *temp;
	// temp = info->start_env;
	// while (temp)
	// {
	// 	printf(MAG"   %s\n"RES, temp->name);
	// 	temp = temp->next;
	// }


	//check_if_builtin(first_cmd, info);

	return (first_cmd);
}

