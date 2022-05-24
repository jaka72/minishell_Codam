#include "../../minishell.h"

int main()
{
	char **arr;
	char *str1;

	arr = malloc(10 * sizeof(char*));
	str1 = malloc(sizeof(char) * 5);

	str1[0] = 'A';
	str1[1] = 'B';
	str1[2] = 'C';
	// //str1 = NULL;
	// printf(YEL"str1[0]: [%c]\n"RES, str1[0]);
	// printf(YEL"str1[1]: [%c]\n"RES, str1[1]);

	arr[0] = str1;
	arr[1] = str1;

	//arr[0] = NULL;
	// arr[1] = NULL;

	int i= 0;
	while (arr[i])
	{
		printf(BLU"i%d [%s]\n"RES, i, arr[i]);
		i++;
	}

	//arr[0] = NULL;
	//arr[1] = NULL;
	//arr = NULL;

	free(arr);
	free(arr[0]);

}