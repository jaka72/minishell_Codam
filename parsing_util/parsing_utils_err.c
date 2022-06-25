/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing_utils_err.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 16:32:52 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#define SYNTAX_ERROR	258

void	ft_lstadd_back(t_cmd **list, t_cmd *newnode)
{
	t_cmd	*temp;

	if (!*list)
	{
		newnode->next = NULL;
		*list = newnode;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = newnode;
	newnode->next = NULL;
}

char	**realloc_array(char **arr, int count)
{
	int		i;
	char	**temp;

	temp = malloc(sizeof(char *) * (count));
	if (temp == NULL)
		return (NULL);
	if (arr == NULL)
		return (temp);
	i = 0;
	while (arr[i])
	{
		temp[i] = ft_strdup(arr[i]);
		if (temp[i] == NULL)
			return (NULL);
		i++;
	}
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = temp;
	return (arr);
}
