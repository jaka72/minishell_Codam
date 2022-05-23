#include "../../minishell.h"

typedef struct s_list
{
	int				value;
	struct s_list	*next;
} t_list;


void	ft_lstadd_back2(t_list *list, t_list *newnode)
{
	t_list *temp;

	temp = list;
	while (temp->next)
		temp = temp->next;
	temp->next = newnode;
	newnode->next = NULL;
}

int main()
{
	t_list	*list;
	t_list	*tmp;
	t_list *temp;

	t_list	*loop;
	t_list	*newnode;

	list = malloc(sizeof(t_list));
	list->value = 1;
	list->next = NULL;

	tmp = malloc(sizeof(t_list));
	tmp->value = 2;
	tmp->next = NULL;

	list->next = tmp;

	loop = list;
	while (loop)
	{
		printf(BLU"value: %d\n"RES, loop->value);
		loop = loop->next;
	}

	newnode = malloc(sizeof(t_list));
	newnode->value = 3;
	newnode->next = NULL;
	//ft_lstadd_back2(list, newnode);

	loop = list;
	while (loop->next)
	{
		printf(RED"value: %d\n"RES, loop->value);
		loop = loop->next;
	}
	loop->next = newnode;
	//newnode->next = NULL;

	loop = list;
	while (loop) // JUST PRINT ////////////////////////////////////////////////////
	{
		printf(GRN"value: %d\n"RES, loop->value);
		loop = loop->next;
	}



	newnode = malloc(sizeof(t_list));
	newnode->value = 4;
	newnode->next = NULL;
	//ft_lstadd_back2(list, newnode);



	temp = list;
	while (temp->next)	 // JUST PRINT ////////////////////////////////////////////////////
		temp = temp->next;

	temp->next = newnode;
	//newnode->next = NULL;
		
	
	
	loop = list;
	while (loop)
	{
		printf(YEL"value: %d\n"RES, loop->value);
		loop = loop->next;
	}

}