#include "../minishell.h"
#define SYNTAX_ERROR	258

void	print_err_msg(char *err_msg)
{
	printf("\nMinishell: %s\n", err_msg);
	g_gl.g_status = SYNTAX_ERROR;
}

void	msg_and_exit(char *err_msg, int exit_code)
{
	write(2, err_msg, ft_strlen(err_msg));
	exit (exit_code);
}

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
