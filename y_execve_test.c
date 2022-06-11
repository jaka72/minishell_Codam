#include "minishell.h"

char *arr0[] = {"../run", NULL};
char *arr[]  = {"./run", NULL};
char *arr1[]  = {"run", NULL};

char *arr2[] = {"builtins/run", NULL};
char *arr3[] = {"./builtins/run", NULL};

char *arr_ls[] = {"/bin/ls", "-la", NULL};

char *arr_folder[] = {"./runx", "-la", NULL};

int main()
{
    int ret;
    int retex;

    printf("A) Thiss is from Y_execve_test.c\n");

    printf(GRN"Acces() returned: %d\n"RES, access("run",   X_OK));
    printf(GRN"Acces() returned: %d\n"RES, access("./run", X_OK));
    printf(GRN"Acces() returned: %d\n"RES, access("../run", X_OK));
    printf(GRN"Acces() returned: %d\n"RES, access("builtins/run", X_OK));
    printf(GRN"Acces() returned: %d\n"RES, access("./builtins/run", X_OK));

    ret = fork();
    if (ret == 0)
    {
        printf("arr0\n");
        execve(arr0[0], arr0, NULL);
    }
    
    wait(NULL);
    ret = fork();
    if (ret == 0)
    {
        printf("arr\n");
        execve(arr[0], arr, NULL);
    }

    wait(NULL);
    ret = fork();
    if (ret == 0)
    {
        printf("arr1\n");
        execve(arr1[0], arr1, NULL);
    }

    wait(NULL);
    ret = fork();
    if (ret == 0)
    {
        printf("arr2\n");
        execve(arr2[0], arr2, NULL);
    }

    wait(NULL);
    ret = fork();
    if (ret == 0)
    {
        printf("arr3\n");
        execve(arr3[0], arr3, NULL);
    }

     wait(NULL);
    ret = fork();
    int acc;
    if (ret == 0)
    {
        printf("arr_folder: ");
        acc = access(arr_folder[0], R_OK || X_OK || W_OK);
        printf(" %d\n", acc);
        retex = execve(arr_folder[0], arr_folder, NULL);
        printf("Execve not exited; but returned %d\n", retex);
    }

    wait(NULL);
    return (0);
}
