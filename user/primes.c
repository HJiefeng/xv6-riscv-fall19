#include "kernel/types.h"
#include "user/user.h"

#define READ_SIDE 0
#define WRITE_SIDE 1
#define COPY_FDS(src, dst) \
    do                    \
    {                     \
        dst[0] = src[0];  \
        dst[1] = src[1];  \
    } while(0)

int main(int argc, char *argv[])
{
    int num = argc >= 2 ? atoi(argv[1]) : 35;
    if (num < 2)
    {
        printf("num should larger than 2\n");
        exit();
    }

    int left_fds[2], right_fds[2];
    int fd1[2];
    if (pipe(fd1) != 0)
    {
        exit();
    }

    int pid;
    if ((pid = fork()) < 0)
    {
        close(fd1[READ_SIDE]);
        close(fd1[WRITE_SIDE]);
        exit();
    }
    else if (pid == 0)
    {
        // child
        COPY_FDS(fd1, left_fds);
        while (1)
        {
            close(left_fds[WRITE_SIDE]);
            int n, p;
            if (read(left_fds[READ_SIDE], &p, sizeof(p)) == 0)
            {
                printf("read EOF \n");
                close(left_fds[READ_SIDE]);
                exit();
            }
            printf("prime %d\n", p);

            int tmp_fds[2];
            if (pipe(tmp_fds) != 0)
            {
                close(left_fds[READ_SIDE]);
                exit();
            }

            int tmp_pid;
            if ((tmp_pid = fork()) < 0)
            {
                close(left_fds[READ_SIDE]);
                close(tmp_fds[READ_SIDE]);
                close(tmp_fds[WRITE_SIDE]);
                exit();
            }
            else if (tmp_pid > 0)
            {
                // parent
                COPY_FDS(tmp_fds, right_fds);
                close(right_fds[READ_SIDE]);
                while (read(left_fds[READ_SIDE], &n, sizeof(n)) != 0)
                {
                    if (n % p != 0)
                    {
                        write(right_fds[WRITE_SIDE], &n, sizeof(n));
                    }
                }
                close(left_fds[READ_SIDE]);
                close(right_fds[WRITE_SIDE]);
                exit();
            }
            // child goto next loop
            close(left_fds[READ_SIDE]);
            COPY_FDS(tmp_fds, left_fds);
        }
    }
    else
    {
        // parent
        COPY_FDS(fd1, right_fds);
        close(right_fds[READ_SIDE]);
        int i;
        for (i = 2; i <= num; i++)
        {
            write(right_fds[WRITE_SIDE], &i, sizeof(i));
        }
        close(right_fds[WRITE_SIDE]);
        wait();
    }

    exit();
}