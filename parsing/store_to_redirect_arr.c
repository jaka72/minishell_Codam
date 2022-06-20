/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   store_to_redirect_arr.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 14:31:57 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/20 08:56:25 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include "make_commands.h"
#include "../minishell.h"

void	store_arrow(t_source *src, t_tmp *t)
{
	t->count = 0;
	t->temp_arr = NULL;
	t->arrow = src->inputline[src->currpos];
}





// SAVE TO CORRECT ARRAY: INFILE, OUTFILE, OR HEREDOC ARRAY
void	store_word_to_array(t_source *src, t_cmd *cmd, t_tmp *t)
// void	store_word_to_array(t_source *src, t_tmp *t)
{
	//printf(CYN"Start store_word, t->count: %d\n"RES, t->count);
	if (t->arrow == '<' && cmd->fd_in == -2)
	{
		t->temp_arr[t->count][0] = '1';
		ft_strlcpy(&t->temp_arr[t->count][1], &src->inputline[t->start], t->len);
		cmd->files = &t->temp_arr[0];
	}
	else if (t->arrow == '>')
	{
		if (cmd->fd_out == -2)
			t->temp_arr[t->count][0] = '2';
		else
			t->temp_arr[t->count][0] = '3';
		ft_strlcpy(&t->temp_arr[t->count][1], &src->inputline[t->start], t->len);
		cmd->files = &t->temp_arr[0];
	}
	else if (t->arrow == '<' && cmd->fd_in == -3)
	{
		ft_strlcpy(t->temp_arr[t->count], &src->inputline[t->start], t->len + 1);
		cmd->heredoc = &t->temp_arr[0];
	}
		

	// ft_strlcpy(t->temp_arr[t->count], &src->inputline[t->start],
	// 	t->len + 1);

	// //printf(CYN"    stored word: [%s] at place %d\n"RES, t->temp_arr[t->count], t->count);
		
	// if (t->arrow == '<' && cmd->fd_in == -2)
	// 	cmd->infile = &t->temp_arr[0];
	// else if (t->arrow == '<' && cmd->fd_in == -3)
	// 	cmd->heredoc = &t->temp_arr[0];
	// else if (t->arrow == '>')
	// 	cmd->outfile = &t->temp_arr[0];
	
	t->temp_arr[t->count + 1] = NULL;
	
	//printf(CYN"    stored word: [%s] at place %d\n"RES, cmd->outfile[t->count], t->count);
	//printf(CYN"    END store_word\n"RES);
	
}






// ALLOCATE CORRECT NR OF PLACES IN **args
// if count == 1, it just started, still empty; Create space for 1st and NULL
// IF MALLOC FAILS, MUST FREE BEFORE EXIT  !!!!
// CALCULATE CORRECT CURR POSITION TO START COPYING THE WORD INTO STRING args[i]
// ALLOCATE SPACE FOR A WORD
// COPY DIRECTLY FROM INPUT LINE, AT CORRECT CURSOR ///////////// HERE
int	store_to_redirect_arr(t_source *src, t_cmd *cmd)
{
	//printf(BLU"START store to redirect\n"RES);

	t_tmp	t;
	char	**temp;
	//int		nr_args;


	store_arrow(src, &t);
	choose_correct_array(src, cmd, &t);

	// t.count = count_elems(cmd->outfile);
	// printf(BLU"    returned t.count: %d\n"RES, t.count);

	// temp = realloc_array(t.temp_arr, t.count);
	temp = realloc_array(t.temp_arr, t.count + 2);
	t.temp_arr = temp;
	if (temp == NULL)
		return (-1);


	skip_white_spaces(src);
	
//	printf(BLU"    A)\n"RES);

	src->currpos++;
	t.len = get_length_of_word(src);
	t.start = src->currpos - t.len + 1;
	// t.temp_arr[t.count - 1] = malloc(sizeof(char) * (t.len + 1));

	if (t.arrow == '<' && cmd->fd_in == -2)  // by kito added to add flag in the begenning
		t.len++;
	else if (t.arrow == '>')
		t.len++;

	t.temp_arr[t.count] = malloc(sizeof(char) * (t.len + 1));
	if (t.temp_arr[t.count] == NULL)
		return (1);

//	printf(BLU"    B)\n"RES);

	store_word_to_array(src, cmd, &t);
	// store_word_to_array(src, &t);
//	printf(BLU"    C)\n"RES);
	//printf(BLU"    saved word [%s] at place %d\n"RES, cmd->infile[t.count], t.count);
//	printf(BLU"    D)\n"RES);
	return (0);
}
