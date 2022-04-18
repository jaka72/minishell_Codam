#ifndef MAKE_COMMAND_H
#define MAKE_COMMAND_H

#include "../jaka_utils/utils.h"

typedef struct s_temporary_array
{
	int		arrow;
	int		len;
	int		start;
	char	**rdr_array;
	int		count;
} t_tmp;

//// MOVED TO UTILS.H, BECAUSE SOME FILES DIDN'T SEE THIS STRUCT AT START
// typedef struct s_cmd
// {
// 	//t_env			*start_env;
// 	char			**args;
// 	int				fd_in;		// fd_in  : default is 0, if "<" -2, if "<<" -3.
// 	int				fd_out;		// fd_out : default is 1, if ">" -2, if ">>" -3.
// 	char			**infile;
// 	char			**outfile;
// 	char			**heredoc;
// 	struct s_cmd	*next;
// 	struct s_cmd	*prev;
// 	int				count_args;			// NEEDED TO KEEP TRACK OF NR OF ELEMENTS,
// 	int				count_infiles;		// TO BE ABLE TO realloc(), EACH TIME: nr + 1
// 	int				count_outfiles;
// 	int				count_heredocs;
// 	//int				temp;		// can delete
// }	t_cmd;



// JUST FOR TESTING
t_cmd	*make_commands(t_source *src);

int		free_commands_list(t_cmd *first_cmd);

//void	free_arrays(t_cmd *cmd);


void	print_command_info(t_cmd *cmd);


// FROM UTILS
//void	skip_white_spaces(t_source *src);


// FROM LIBFT
//size_t	ft_strlcpy(char *dest, const char *src, size_t size);



#endif