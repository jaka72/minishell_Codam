#ifndef UTILS_H
#define UTILS_H

# include "colors.h"
//# include "from_libft.h"
# include "other_libraries.h"



# define SYNTAX_ERROR 	258
# define ENDOFLINE		(-1)
# define NOINPUT		(-1)


typedef struct s_source
{
	char	*inputline;
	long	inputline_size;	// Length of the input text
	long	currpos;		// index of char position in src->inputline
} t_source;



typedef struct s_cmd
{
	//t_env			*start_env;
	char			**args;
	int				fd_in;		// fd_in  : default is 0, if "<" -2, if "<<" -3.
	int				fd_out;		// fd_out : default is 1, if ">" -2, if ">>" -3.
	char			**infile;
	char			**outfile;
	char			**heredoc;
	struct s_cmd	*next;
	struct s_cmd	*prev;
	int				count_args;			// NEEDED TO KEEP TRACK OF NR OF ELEMENTS,
	int				count_infiles;		// TO BE ABLE TO realloc(), EACH TIME: nr + 1
	int				count_outfiles;
	int				count_heredocs;
	//int				temp;		// can delete
}	t_cmd;


# include "../check_syntax/check_syntax.h"
# include "../parsing/make_commands.h"
# include "../builtins/builtins.h"



// UTILS //////////////////////////
int		is_allowed_char(int c);
int		is_valid_filename_char(int c);
int		print_err_msg(char *err_msg);

char	peek_next_char(t_source *src);
char	get_next_char(t_source *src);
void	skip_white_spaces(t_source *src);





// FROM LIBFT //////////////////
int		ft_isalpha(int c);
int		ft_isspace(int c);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
//char	**ft_split(char	const *s, char c);
//char	*ft_strjoin(char const *s1, char const *s2);


#endif
