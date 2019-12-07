#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    // copy argv
    char **newv = malloc((MAXARG+1) * sizeof(*newv));
    memmove(newv, argv, sizeof(*newv) * argc);
    newv[argc] = 0;

    int i;
    int curr_arg;
    int pid;
    int is_end;
    while (1)
    {
        is_end = 0;
        curr_arg = argc;
        char buf[1024];
        char c;
        while (!is_end && curr_arg < MAXARG) {
            for (i = 0; i + 1 < sizeof(buf);)
            {
                if (read(0, &c, 1) < 1)
                    break;
                if (c == '\n' || c == '\r') {
                    is_end = 1;
                    break;
                } else if (c == ' ') {
                    break;
                } else {
                    buf[i++] = c;
                }
            }
            
            if (i != 0) {
                buf[i] = '\0';
                char *new_arg = malloc(i + 1);
                memmove(new_arg, buf, i+1);
                newv[curr_arg++] = new_arg;
            }
        }

        if (curr_arg == argc)
            continue;
        newv[curr_arg] = 0;

        // fork exec
        if ((pid = fork()) < 0)
        {
            printf("xargs: fork error\n");
            exit();
        }
        else if (pid > 0)
        {
            // parent
            wait();
        }
        else
        {
            // child
            exec(newv[1], &newv[1]);
            exit();
        }
    }
    free(newv);
    exit();
}