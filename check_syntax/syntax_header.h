#ifndef SYNTAX_HEADER_H
#define SYNTAX_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h> 
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>
#include "colors.h"

# define SYNTAX_ERROR 	222 // HOW TO OUTPUT 258 ???
# define ENDOFLINE		(-1)
# define NOINPUT		(-1)

// SYNTAX ERROR MESSAGES
#define MSG_QUOTES "Syntax error with QUOTES or UNSUPPORTED CHARS"
#define	MSG_PIPES "Syntax error with PIPES"
#define	MSG_REDIRECTS "Syntax error with REDIRECTS"


///////////////////////////////////////////////////////////////////////


typedef struct s_source
{
	char	*inputline;
	long	inputline_size;	// Length of the input text
	long	currpos;		// index of char position in src->inputline

} t_source;


// FUNCTIONS ////////////////////
char	peek_next_char(t_source *src);
char	get_next_char(t_source *src);
void	skip_white_spaces(t_source *src);


// INITIAL CHECK
int		check_pipes(t_source *src);
int		check_redirects(t_source *src);
int		check_quotes(t_source *src);


// UTILS //////////////////////////
int		is_allowed_char(int c);
int		print_err_msg(t_source *src, char *err_msg);


// FROM LIBFT //////////////////
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char	const *s, char c);
int		ft_isalpha(int c);
int		ft_isspace(int c);


#endif
