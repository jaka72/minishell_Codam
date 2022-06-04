/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   make_commands.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 13:39:36 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/04 12:05:16 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "make_commands.h"

char	**realloc_array(char **arr, int count)
{
	printf(CYN"Start realloc, count: %d\n"RES, count);
	int		i;
	char	**temp;

	temp = malloc(sizeof(char *) * (count));
	if (temp == NULL)
		return (NULL);
	
	if (arr == NULL)
	{
		printf(CYN"   arr was NULL, now malloced * 2\n"RES);
		return (temp);
	}
	
	i = 0;
	while (arr[i])
	{
		printf(CYN"   loop i%d\n"RES, i);

		temp[i] = ft_strdup(arr[i]);
		i++;
	}
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = temp;

	printf(CYN"END realloc;\n"RES);

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

	nr_args = count_args(cmd->args);
	printf(CYN"START store into command, counted args: %d\n"RES, nr_args);

	temp = realloc_array(cmd->args, nr_args + 2);
	cmd->args = temp;

	len = get_length_of_word(src);
	start = src->currpos - len + 1;
	
	cmd->args[nr_args] = malloc(sizeof(char) * (len + 1));
	if (cmd->args[nr_args] == NULL)
		return (1);
		
	ft_strlcpy(cmd->args[nr_args], &src->inputline[start], len + 1);
	cmd->args[nr_args + 1] = NULL;
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
			store_to_redirect_arr(src, cmd);
		}
		else if (is_valid_filename_char(ch) && ch != '\0')
		{
			// cmd->count_args++;
			printf(MAG"   Found valid ch [%c]\n"RES, ch);
			store_into_command_arr(src, cmd);
		}
		else if (ch == '|')
			break ;
		else if (ch == '\0')
			return (1);
	}
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
		printf(YEL"Loop make commands a) \n"RES);

		new_cmd = malloc(sizeof(t_cmd));
		if (new_cmd == NULL)
			exit (1); // some message and free all !!!
		// set args to NULL, TO BE ABLE TO DETECT LATER WHEN COUNTING, THAT IT IS EMPTY
		new_cmd->args = NULL;
		
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
	return (first_cmd);
}
