/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_syntax.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 16:42:27 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define SYNTAX_ERROR	258
#define MSG_SYNTAX		"Syntax error"

int	check_syntax_errors(t_source *src, int *stat)
{
	src->currpos = 0;
	src->inputline_size = ft_strlen(src->inputline);
	if (src->inputline_size == 0)
		return (0);
	if (check_quotes(src))
	{
		print_err_msg(MSG_SYNTAX);
		*stat = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}
	if (check_pipes(src))
	{
		print_err_msg(MSG_SYNTAX);
		*stat = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}
	if (check_redirects(src))
	{
		print_err_msg(MSG_SYNTAX);
		*stat = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}
	return (0);
}
