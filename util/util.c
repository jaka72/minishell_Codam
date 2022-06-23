/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 16:13:05 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

char	*free_return_null(char *text)
{
	if (text != NULL)
		free(text);
	return (NULL);
}

char	*ft_add_c_free(char *s1, char c)
{
	char	*str;
	int		i;
	int		j;

	if (s1 == NULL)
		return (NULL);
	i = ft_strlen(s1);
	str = malloc(sizeof(char) * (i + 1 + 1));
	j = 0;
	if (!str)
		return (NULL);
	while (j < i)
	{
		str[j] = s1[j];
		j++;
	}
	str[j] = c;
	j++;
	str[j] = '\0';
	free(s1);
	return (str);
}

void	clean_fd(void)
{
	t_cmd	*current;

	current = g_gl.start_cmd;
	while (current)
	{
		if (current->fd_in > 1)
		{
			close(current->fd_in);
			current->fd_in = 0;
		}
		if (current->fd_out > 1)
		{
			close(current->fd_out);
			current->fd_out = 1;
		}
		current = current->next;
	}
	if (g_gl.ini_fd[0] > 1)
		close(g_gl.ini_fd[0]);
	if (g_gl.ini_fd[1] > 1)
		close(g_gl.ini_fd[1]);
}

int	clean_data(int status, char *text)
{
	if (text != NULL)
		write(STDOUT_FILENO, text, ft_strlen(text));
	free_envlist();
	clean_fd();
	rl_clear_history();
	tcsetattr(0, 0, &g_termios_saved);
	return (status);
}

int	wait_return(void)
{
	while (wait(NULL) > 0)
		;
	return (1);
}
