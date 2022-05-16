#ifndef MAKE_COMMAND_H
#define MAKE_COMMAND_H

# include "../minishell.h"

//# include "../jaka_utils/utils.h"

typedef struct s_temporary_array
{
	int		arrow;
	int		len;
	int		start;
	char	**rdr_array;
	int		count;
} t_tmp;


// MAKE COMMANDS
t_cmd	*make_commands(t_source *src, t_infos *info);
int		free_commands_list(t_cmd *first_cmd);
void	print_command_info(t_cmd *cmd);
//void	free_arrays(t_cmd *cmd);


// UTILS
void	init_values(t_cmd *cmd);
int		check_each_command(t_cmd *cmd, t_infos *info);
int		check_if_builtin(t_cmd *cmd, t_infos *info);
void	ft_lstadd_back(t_cmd **list, t_cmd *newnode);
int		get_length_of_word(t_source *src);




// FROM UTILS
//void	skip_white_spaces(t_source *src);

// FROM LIBFT
//size_t	ft_strlcpy(char *dest, const char *src, size_t size);


#endif