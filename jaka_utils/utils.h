#ifndef UTILS_H
#define UTILS_H

# include "colors.h"
# include "other_libraries.h"
# include "../minishell.h"

# define SYNTAX_ERROR 	258
# define ENDOFLINE		(-1)
# define NOINPUT		(-1)

// UTILS //////////////////////////
int		is_allowed_char(int c);
int		is_valid_filename_char(int c);
void	print_err_msg(char *err_msg);
char	peek_next_char(t_source *src);
char	get_next_char(t_source *src);
void	skip_white_spaces(t_source *src);

#endif
