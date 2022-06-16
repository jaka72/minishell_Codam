#include "../../minishell.h"

char *arr0[] = {"../run", NULL};
char *arr[]  = {"./run", NULL};
char *arr1[]  = {"run", NULL};

char *arr2[] = {"builtins/run", NULL};
char *arr3[] = {"./builtins/run", NULL};

char *arr_ls[] = {"/bin/ls", "-la", NULL};


// char *arr_folder[] = {"builtins/runno_perm", NULL};

int main(int argc, char **argv)
{
    int ret_fork;
    char *arr_folder[2];


    // arr_folder[0] = "ls";
    arr_folder[1] = NULL;

    if (argc == 2)
        arr_folder[0] = argv[1];
    else
        arr_folder[0] = "./run";

    printf("A) This is from Y_execve_test.c: args[0]: [%s]\n", arr_folder[0]);

    // printf(GRN"Acces() returned: %d\n"RES, access("run",   X_OK));
    // printf(GRN"Acces() returned: %d\n"RES, access("./run", X_OK));
    // printf(GRN"Acces() returned: %d\n"RES, access("../run", X_OK));
    // printf(GRN"Acces() returned: %d\n"RES, access("builtins/run", X_OK));
    // printf(GRN"Acces() returned: %d\n"RES, access("./builtins/run", X_OK));

    // ret = fork();
    // if (ret == 0)
    // {
    //     printf("arr0\n");
    //     execve(arr0[0], arr0, NULL);
    // }
    
    // wait(NULL);
    // ret = fork();
    // if (ret == 0)
    // {
    //     printf("arr\n");
    //     execve(arr[0], arr, NULL);
    // }

    // wait(NULL);
    // ret = fork();
    // if (ret == 0)
    // {
    //     printf("arr1\n");
    //     execve(arr1[0], arr1, NULL);
    // }

    // wait(NULL);
    // ret = fork();
    // if (ret == 0)
    // {
    //     printf("arr2\n");
    //     execve(arr2[0], arr2, NULL);
    // }

    // wait(NULL);
    // ret = fork();
    // if (ret == 0)
    // {
    //     printf("arr3\n");
    //     execve(arr3[0], arr3, NULL);
    // }

    //  wait(NULL);
    ret_fork = fork();
    int ret_access;
    int ret_execve;
    if (ret_fork == 0)
    {


        // printf("arr_folder: [%s]\n", arr_folder[0]);
        // ret_access = access(arr_folder[0], R_OK || X_OK || W_OK);
        // if (ret_access != 0)
        // {
        //     printf("After access, ret = %d, errno = %d\n", ret_access, errno);
        //     if (errno == 2)
        //     {
        //         //perror("   perror: ");
        //         //printf("   access: No such file ...\n");
        //         // exit (127);
        //     }
        //     if (errno == 13)
        //     {
        //         printf("access: Permission denied\n");
        //         exit (126);
        //     }
        // }


        ret_execve = execve(arr_folder[0], arr_folder, NULL);
        printf(GRN"After execve, ret = %d, errno = %d\n"RES, ret_execve, errno);
        
        if (ret_execve != 0)
        {
            perror(GRN"   Errno after execve:"RES);
            printf(GRN"   Errno: %d"RES, errno);
            printf("\n");
            // if (strchr(arr_folder[0], '/') != NULL)
            //     printf(GRN"   execve: No such file ..., errno: %d\n"RES, errno);
            // else
            //     printf(GRN"   execve: Command not found, errno: %d\n"RES, errno);
            exit (123);
        }        
    }

    wait(NULL);
    return (0);
}
