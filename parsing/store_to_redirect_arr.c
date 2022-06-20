#include "../minishell.h"

static	void	store_arrow(t_source *src, t_tmp *t)
{
	t->count = 0;
	t->temp_arr = NULL;
	t->arrow = src->inputline[src->currpos];
}

static	void	store_word_to_array(t_source *src, t_cmd *cmd, t_tmp *t)
{
	if (t->arrow == '<' && cmd->fd_in == -2)
	{
		t->temp_arr[t->count][0] = '1';
		ft_strlcpy(&t->temp_arr[t->count][1],
			&src->inputline[t->start], t->len);
		cmd->files = &t->temp_arr[0];
	}
	else if (t->arrow == '>')
	{
		if (cmd->fd_out == -2)
			t->temp_arr[t->count][0] = '2';
		else
			t->temp_arr[t->count][0] = '3';
		ft_strlcpy(&t->temp_arr[t->count][1],
			&src->inputline[t->start], t->len);
		cmd->files = &t->temp_arr[0];
	}
	else if (t->arrow == '<' && cmd->fd_in == -3)
	{
		ft_strlcpy(t->temp_arr[t->count],
			&src->inputline[t->start], t->len + 1);
		cmd->heredoc = &t->temp_arr[0];
	}
	t->temp_arr[t->count + 1] = NULL;
}
// SAVE TO CORRECT ARRAY: INFILE, OUTFILE, OR HEREDOC ARRAY

int	store_to_redirect_arr(t_source *src, t_cmd *cmd)
{
	t_tmp	t;
	char	**temp;

	store_arrow(src, &t);
	choose_correct_array(src, cmd, &t);
	temp = realloc_array(t.temp_arr, t.count + 2);
	t.temp_arr = temp;
	if (temp == NULL)
		return (-1);
	skip_white_spaces(src);
	src->currpos++;
	t.len = get_length_of_word(src);
	t.start = src->currpos - t.len + 1;
	if (t.arrow == '<' && cmd->fd_in == -2)
		t.len++;
	else if (t.arrow == '>')
		t.len++;
	t.temp_arr[t.count] = malloc(sizeof(char) * (t.len + 1));
	if (t.temp_arr[t.count] == NULL)
		return (1);
	store_word_to_array(src, cmd, &t);
	return (0);
}
// ALLOCATE CORRECT NR OF PLACES IN **args
// if count == 1, it just started, still empty; Create space for 1st and NULL
// IF MALLOC FAILS, MUST FREE BEFORE EXIT  !!!!
// CALCULATE CORRECT CURR POSITION TO START COPYING THE WORD INTO STRING args[i]
// ALLOCATE SPACE FOR A WORD
// COPY DIRECTLY FROM INPUT LINE, AT CORRECT CURSOR ///////////// HERE