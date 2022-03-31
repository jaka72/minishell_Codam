#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* environ */

int main()
{
    extern char **environ;
    char **env = environ;

    while(*env) {
        const char *val;

        val = *env;
        if (val) {
            printf("%s\n", val);
        }
        env++;
    }

    return 0;
}