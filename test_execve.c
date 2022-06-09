#include "minishell.h"

char *arr[] = {"../x_test_command", NULL};
char *arr_ls[] = {"/bin/ls", "-la", NULL};

int main()
{
    int i = 0;
    printf("B) Thiss is from test execce.c\n");
    // while (i < 10)
    // {
    //     i++;
    // }


    execve(arr[0], arr, NULL);

    //execve("/bin/ls", arr_ls, NULL);


    return (0);
}
