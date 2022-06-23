/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   check_files.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 16:59:37 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

static int	check_infile(char *expanded)
{
	if (access(expanded, F_OK) != 0 || (access(expanded, F_OK) == 0
			&& access(expanded, R_OK) < 0))
	{
		return_perr(-4, expanded);
		free(expanded);
		return (-4);
	}
	return (0);
}

static int	check_outfile(char *expanded)
{
	int	j;

	j = 0;
	if (access(expanded, F_OK) == 0 && access(expanded, W_OK) < 0)
	{
		return_perr(-4, expanded);
		free(expanded);
		return (-4);
	}
	if (access(expanded, F_OK) != 0)
	{
		j = open(expanded, O_CREAT, 0666);
		if (j < 0)
		{
			return_perr(-4, expanded);
			free(expanded);
			return (-4);
		}
		close(j);
	}
	return (0);
}

static char	*flag_and_expand(char *expanded, char *original)
{
	char	*temp;

	temp = NULL;
	temp = malloc(sizeof(char) * ft_strlen(expanded) + 2);
	if (temp == NULL)
		errtx_all_free_exit(1, "malloc for temp");
	temp[0] = original[0];
	ft_strlcpy(&temp[1], expanded, ft_strlen(expanded) + 1);
	free(expanded);
	free(original);
	return (temp);
}

int	check_file_access(t_cmd	*current, int *ex_stat)
{
	char	*expanded;
	int		i;

	expanded = NULL;
	i = 0;
	if (current->files == NULL)
		return (0);
	while (current->files[i])
	{
		expanded = check_expand_file(&current->files[i][1], ex_stat);
		if (expanded == NULL)
			return (return_errtx(-4, "ambiguous redirect\n"));
		if (current->files[i][0] == '1' && check_infile(expanded) != 0)
			return (-4);
		if (current->files[i][0] == '2' || current->files[i][0] == '3')
		{
			if (check_outfile(expanded) != 0)
				return (-4);
		}
		current->files[i] = flag_and_expand(expanded, current->files[i]);
		i++;
	}
	return (0);
}
