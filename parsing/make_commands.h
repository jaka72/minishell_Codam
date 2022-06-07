#ifndef MAKE_COMMAND_H
#define MAKE_COMMAND_H

# include "../minishell.h"

typedef struct s_temporary_array
{
	int		arrow;
	int		len;
	int		start;
	char	**temp_arr;
	int		count;
} t_tmp;

// MAKE COMMANDS
// t_cmd	*make_commands(t_source *src);
t_cmd	*make_commands(t_source *src, t_infos *info);
int		free_commands_list(t_cmd *first_cmd);
void	print_command_info(t_cmd *cmd);
void	choose_correct_array(t_source *src, t_cmd *cmd, t_tmp *t);
int		store_to_redirect_arr(t_source *src, t_cmd *cmd);
char	**realloc_array(char **arr, int count);

// UTILS
void	init_values(t_cmd *cmd);
int		check_if_builtin(t_cmd *cmd);
int		exec_builtin(t_cmd *cmd, t_infos *info, t_cmd *list);
// int		exec_builtin(t_cmd *cmd, t_infos *info);
void	ft_lstadd_back(t_cmd **list, t_cmd *newnode);
int		get_length_of_word(t_source *src);

#endif
