#include "../minishell.h"

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
