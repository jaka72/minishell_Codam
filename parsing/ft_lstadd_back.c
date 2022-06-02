/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstadd_back.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jaka <jaka@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 13:18:57 by jaka          #+#    #+#                 */
/*   Updated: 2022/06/01 10:06:19 by jmurovec      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h"
#include "make_commands.h"

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
	{
		temp = temp->next;
	}
	temp->next = newnode;
	newnode->next = NULL;
}
