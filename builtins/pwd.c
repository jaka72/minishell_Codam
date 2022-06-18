/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 12:43:45 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/15 15:49:52 by jmurovec      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

// free(current_path); // DOES IT NEED TO BE FREED ???
int	run_pwd_builtin(void)
{
	char	buff[PATH_MAX];
	char	*current_path;

	current_path = getcwd(buff, PATH_MAX);
	if (current_path == NULL)
		return (1);
	else
	{
		write(1, current_path, ft_strlen(current_path));
		write(1, "\n", 1);
		return (0);
	}
}
