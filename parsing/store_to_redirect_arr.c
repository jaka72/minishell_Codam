/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   store_to_redirect_arr.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 14:31:57 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/03 17:41:16 by jaka          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "make_commands.h"

void	store_arrow(t_source *src, t_tmp *t)
{
	t->count = 0;
	t->temp_arr = NULL;
	t->arrow = src->inputline[src->currpos];
}

// SAVE TO CORRECT ARRAY: INFILE, OUTFILE, OR HEREDOC ARRAY
void	store_word_to_array(t_source *src, t_cmd *cmd, t_tmp *t)
{
	ft_strlcpy(t->temp_arr[t->count - 1], &src->inputline[t->start],
		t->len + 1);
	if (t->arrow == '<' && cmd->fd_in == -2)
		cmd->infile = &t->temp_arr[0];
	else if (t->arrow == '<' && cmd->fd_in == -3)
		cmd->heredoc = &t->temp_arr[0];
	else if (t->arrow == '>')
		cmd->outfile = &t->temp_arr[0];
	t->temp_arr[t->count] = NULL;
}

// ALLOCATE CORRECT NR OF PLACES IN **args
// if count == 1, it just started, still empty; Create space for 1st and NULL
// IF MALLOC FAILS, MUST FREE BEFORE EXIT  !!!!
// CALCULATE CORRECT CURR POSITION TO START COPYING THE WORD INTO STRING args[i]
// ALLOCATE SPACE FOR A WORD
// COPY DIRECTLY FROM INPUT LINE, AT CORRECT CURSOR ///////////// HERE
int	store_to_redirect_arr(t_source *src, t_cmd *cmd)
{
	printf(BLU"Store to redirect\n"RES);

	t_tmp	t;
	char	**temp;
	int		nr_args;

	nr_args = count_infiles(cmd->infile);


	store_arrow(src, &t);
	choose_correct_array(src, cmd, &t);

	printf(BLU"    store A), nr = %d\n"RES, nr_args);

	
	// if (t.count == 1)
	if (nr_args == 0)
	{
		printf(BLU"    nr == 0\n"RES);
		t.temp_arr = malloc(sizeof(char *) * 2);
		if (t.temp_arr == NULL)
			return (1);
		t.temp_arr[0] = ft_strdup(""); // CHECK IF SUCCESS
		t.temp_arr[1] = NULL;
	}
	else
	{
		printf(BLU"    else, nr == %d\n"RES, nr_args);
		temp = realloc_array(t.temp_arr, t.count);
		t.temp_arr = temp;
	}

	nr_args = count_infiles(t.temp_arr);

	printf(BLU"    store B), nr = %d\n"RES, nr_args);


	skip_white_spaces(src);
	src->currpos++;
	t.len = get_length_of_word(src);


	free(t.temp_arr[nr_args - 1]);

	
	t.start = src->currpos - t.len + 1;
	t.temp_arr[t.count - 1] = malloc(sizeof(char) * (t.len + 1));
	if (t.temp_arr[t.count - 1] == NULL)
		return (1);
	store_word_to_array(src, cmd, &t);
	return (0);
}
