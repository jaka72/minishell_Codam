#include "minishell.h"

int main()
{
	char **arr;

	arr = malloc(sizeof(char *) * 5);

	


	int i = 0;
	while (i < 5)
	{
		arr[i] = malloc(sizeof(char) * 10);
		i++;
	}

	arr[4] = NULL;

	arr[0][0] = '\0';
	arr[0][1] = 'b';




	printf("a)\n");
	//printf("  arr[0]: [%s]\n", arr[0]);

	//free(arr[0]);
	
	
	i = 0;
	while (arr[i])
	{
		printf("arr[%d]: [%s]  [%p]\n", i, arr[i], arr[i]);
		i++;
	}


	printf("b)\n");

	// arr[0] = "";
	// arr[1] = "";
	// arr[2] = "";
	// arr[3] = "";
	// arr[4] = NULL;

	printf("c)\n");




	i = 0;
	while (arr[i])
	// while (i < 5)
	{
		printf("     free arr[%d]: [%s]  [%p]\n", i, arr[i], arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
}