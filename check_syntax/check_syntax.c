/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_syntax.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmurovec <jmurovec@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/02 10:27:00 by jmurovec      #+#    #+#                 */
/*   Updated: 2022/06/02 11:10:58 by jmurovec      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "check_syntax.h"

int	check_syntax_errors(t_source *src)
{
	src->currpos = 0;
	src->inputline_size = ft_strlen(src->inputline);
	if (src->inputline_size == 0)
		return (0);
	if (check_quotes(src) != 0)
	{
		print_err_msg(MSG_SYNTAX);
		return (SYNTAX_ERROR);
	}
	if (check_pipes(src) != 0)
	{
		print_err_msg(MSG_SYNTAX);
		return (SYNTAX_ERROR);
	}
	if (check_redirects(src) != 0)
	{
		print_err_msg(MSG_SYNTAX);
		return (SYNTAX_ERROR);
	}
	return (0);
}
