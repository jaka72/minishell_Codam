#include "../minishell.h"

char	**realloc_array(char **arr, int count)
{
	int		i;
	char	**temp;

	temp = malloc(sizeof(char *) * (count));
	if (temp == NULL)
		return (NULL);
	if (arr == NULL)
		return (temp);
	i = 0;
	while (arr[i])
	{
		temp[i] = ft_strdup(arr[i]);
		if (temp[i] == NULL)
			return (NULL);
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
	return (arr);
}

static	int	store_into_command_arr(t_source *src, t_cmd *cmd)
{
	int		len;
	int		start;
	char	**temp;
	int		nr_args;

	nr_args = count_elems(cmd->args);
	temp = realloc_array(cmd->args, nr_args + 2);
	cmd->args = temp;
	if (temp == NULL)
		return (-1);
	len = get_length_of_word(src);
	start = src->currpos - len + 1;
	cmd->args[nr_args] = malloc(sizeof(char) * (len + 1));
	if (cmd->args[nr_args] == NULL)
		return (-1);
	ft_strlcpy(cmd->args[nr_args], &src->inputline[start], len + 1);
	cmd->args[nr_args + 1] = NULL;
	return (0);
}
// ALLOCATES CORRECT NR OF PLACES IN **args
// If just started, it's still empty; Create space for 1st and NULL
// CALCULATE CORRECT CURR POSITION TO START COPYING THE WORD INTO STRING args[i]
// ALLOCATE SPACE FOR A WORD
// COPY DIRECTLY FROM INPUT LINE, AT CORRECT CURSOR ///////////// HERE

static	int	select_and_store_words(t_source *src, t_cmd *cmd)
{
	int	ch;

	init_values(cmd);
	while (1)
	{
		skip_white_spaces(src);
		src->currpos++;
		ch = src->inputline[src->currpos];
		if (ch == '<' || ch == '>')
		{	
			if (store_to_redirect_arr(src, cmd) == -1)
				return (-1);
		}
		else if (is_valid_filename_char(ch) && ch != '\0')
		{
			if (store_into_command_arr(src, cmd) == -1)
				return (-1);
		}
		else if (ch == '|')
			break ;
		else if (ch == '\0')
			return (1);
	}
	return (0);
}

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
			msg_and_exit("cmd: malloc failed\n", 1);
		new_cmd->args = NULL;
		ret = select_and_store_words(src, new_cmd);
		ft_lstadd_back(&first_cmd, new_cmd);
		if (ret == 1)
			break ;
		if (ret == -1)
			errtx_all_free_exit(1, "make_commands: malloc failed\n");
		i++;
	}
	return (first_cmd);
}
// REMOVE i  and printing info
// set args to NULL, TO BE ABLE TO DETECT LATER WHEN COUNTING, THAT IT IS EMPTY
// t_cmd	*make_commands(t_source *src)
//	ret == -1 means Malloc failed