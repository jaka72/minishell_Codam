/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   make_commands.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 13:39:36 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/03 17:36:20 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "make_commands.h"

char	**realloc_array(char **arr, int count)
{
	printf(RED"   START realloc count: %d\n"RES, count);

	int		i;
	char	**temp;

	temp = malloc(sizeof(char *) * (count + 1));
	if (temp == NULL)
		return (NULL);
	// temp[count] = "";
	//temp[count + 1] = NULL;
	i = 0;
	while (arr[i])
	{
		printf(RED"      loop i: %d\n"RES, i);
		temp[i] = ft_strdup(arr[i]);
		// maybe here needs to be checked if NULL
		//printf(RED"      loop i: %d,   temp[i]: [%s]\n"RES, i, temp[i]);
		i++;
	}
	
	
	//printf(RED"    ..... a realloc i: %d\n"RES, i);
	// temp[i][0] = '\0'; // ITS NOT MALLOCED YET

	// LAST ELEMENT MUST BE EMPTY, BUT NOT NULL, THE LAST MUST BE NULL
	temp[i] = ft_strdup("");		// need to check success
	//printf(RED"    ..... aa realloc i: %d\n"RES, i);
	temp[i + 1] = NULL;
	//printf(RED"    ..... b realloc i: %d\n"RES, i);



	i = 0;
	while (arr[i])
	{
		printf(GRN"                  i:%d, [%s]\n"RES, i, arr[i]);
		free(arr[i]);
		i++;
	}
	//printf(RED"    ..... c realloc i: %d\n"RES, i);
	free(arr);
	arr = temp;


	// test, print all new array arr
	i = 0;
	while (arr[i])
	{
		printf(WHT"      i%d[%s]"RES, i, arr[i]);
		//free(arr[i]);
		i++;
	}
	//printf(RED"\n    .....  END realloc i: %d\n"RES, i);
	return (arr);
}

// ALLOCATES CORRECT NR OF PLACES IN **args
// If just started, it's still empty; Create space for 1st and NULL
// CALCULATE CORRECT CURR POSITION TO START COPYING THE WORD INTO STRING args[i]
// ALLOCATE SPACE FOR A WORD
// COPY DIRECTLY FROM INPUT LINE, AT CORRECT CURSOR ///////////// HERE
// Should malloc error give message ?
int	store_into_command_arr(t_source *src, t_cmd *cmd)
{
	
	int		len;
	int		start;
	char	**temp;
	int		nr_args;

	nr_args = count_args(cmd);
	printf(CYN"START store into command, nr_args: %d\n"RES, nr_args);

	if (cmd->args == NULL)
	{
		//printf(CYN"   args is NULL\n"RES);
		cmd->args = malloc(sizeof(char *) * 2);
		if (cmd->args == NULL)
			return (1);
		//cmd->args[0] = malloc(sizeof(char)); 	// need to check success
		//cmd->args[0][0] = '\0';					// could also use strdup ???
		cmd->args[0] = ft_strdup("");
		cmd->args[1] = NULL;
		//printf(CYN"   a args[0]: [%s]\n"RES, cmd->args[0]);
		//printf(CYN"   b args[1]: [%s]\n"RES, cmd->args[1]);
		//printf(CYN"   c count: %d\n"RES, nr_args);
	}
	else
	{
		//printf(CYN"   args is ELSE\n"RES);
		// temp = realloc_array(cmd->args, cmd->count_args);
		temp = realloc_array(cmd->args, nr_args + 1);
		cmd->args = temp;
	}

	nr_args = count_args(cmd);
	//printf(CYN"   2nd count: %d\n"RES,	nr_args);
	
	
	
	len = get_length_of_word(src);
	start = src->currpos - len + 1;

	// free the one that was malloced above with empty ""
	free(cmd->args[nr_args - 1]);

	// MALLOC AGAIN WITH CORRECT NEW WORD LENGTH
	// cmd->args[cmd->count_args - 1] = malloc(sizeof(char) * (len + 1));
	cmd->args[nr_args - 1] = malloc(sizeof(char) * (len + 1));
	// if (cmd->args[cmd->count_args - 1] == NULL)
	if (cmd->args[nr_args - 1] == NULL)
		return (1);


	//printf(CYN"   new word will to strlcpy to place %d\n"RES,	nr_args - 1);


	// ft_strlcpy(cmd->args[cmd->count_args - 1], &src->inputline[start], len + 1);
	ft_strlcpy(cmd->args[nr_args - 1], &src->inputline[start], len + 1);
	// cmd->args[cmd->count_args] = NULL;
	cmd->args[nr_args] = NULL;

	
	printf(CYN"   new word at place %d: [%s]\n"RES, nr_args - 1, cmd->args[nr_args - 1]);
	return (0);
}

int	select_and_store_words(t_source *src, t_cmd *cmd)
{

	int	ch;

	init_values(cmd);
	while (1)
	{
		printf(MAG"Loop select words \n"RES);
		skip_white_spaces(src);
		src->currpos++;
		ch = src->inputline[src->currpos];
		if (ch == '<' || ch == '>')
		{	
			printf(MAG"   found < > \n"RES);
			store_to_redirect_arr(src, cmd);
		}
		else if (is_valid_filename_char(ch) && ch != '\0')
		{
			//cmd->count_args++;
			printf(MAG"   A) Found valid ch [%c]\n"RES, ch);
			store_into_command_arr(src, cmd);
			//printf("    B)\n");
		}
		else if (ch == '|')
			break ;
		else if (ch == '\0')
		{
			//printf(MAG"   found \\0: break \n"RES);
			return (1);
		}
	}
	printf(MAG"   end of select words \n"RES);
	return (0);
}

// REMOVE i  and printing info
t_cmd	*make_commands(t_source *src)
{
	int		i;
	int		ret;
	t_cmd	*first_cmd;
	t_cmd	*new_cmd;

	if (src->inputline_size == 0)
		return (0);
	src->currpos = -1;
	first_cmd = NULL;
	i = 0;
	while (1)
	{
		new_cmd = malloc(sizeof(t_cmd));
		if (new_cmd == NULL)
			exit (1); // some message and free all !!!
		
		// set args to NULL
		new_cmd->args = NULL;

		// AT START, IMEDDIATELY AFTER MALLOCING, THE STRUCT IS APPARENTLY NULL
		printf(YEL"Loop make commands a) \n"RES);
		//printf(YEL"    newcmd->args [%p]\n"RES, new_cmd->args);		

		ret = select_and_store_words(src, new_cmd);
		
		
		print_command_info(new_cmd);
		
		
		if (ret == 1)
		{
			ft_lstadd_back(&first_cmd, new_cmd);
			break ;
		}
		ft_lstadd_back(&first_cmd, new_cmd);
		i++;
	}
	//printf(YEL"END make commands\n"RES);
	return (first_cmd);
}
