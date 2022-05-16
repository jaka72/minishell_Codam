#ifndef MAIN_H
#define MAIN_H

#include "../jaka_utils/utils.h"

// SYNTAX ERROR MESSAGES
#define MSG_QUOTES "Syntax error with QUOTES"
#define	MSG_PIPES "Syntax error with PIPES"
#define	MSG_REDIRECTS "Syntax error with REDIRECTS"
#define	MSG_UNSUPPORTED "Syntax error: found char unsupported by Minishell"


// INITIAL CHECK
int	check_syntax_errors(t_source *src, t_infos *info);
int	check_unsuported_chars(t_source *src);
int	check_pipes(t_source *src);
int	check_redirects(t_source *src);
int	check_quotes(t_source *src);

void skip_alphas_spaces_pipes(t_source *src);
int	is_space_alpha_or_pipe(t_source *src, int *c);
int	check_char_after_space(t_source *src, int *c);


#endif
