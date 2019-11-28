#include "kernel/types.h"
#include "user/user.h"

#define READ_SIDE 0
#define WRITE_SIDE 1

int main(int argc, char *argv[])
{
    int fd1[2], fd2[2];
    int child_pid;
    if (pipe(fd1) != 0 || pipe(fd2) != 0)
    {
        printf("create pipe error\n");
        exit();
    }
    
    if ((child_pid = fork()) < 0) {
        printf("fork error\n");
        exit();
    } else if (child_pid == 0){
        char msg;
        read(fd1[READ_SIDE], &msg, 1);
        printf("%d: received ping\n", getpid());
        write(fd2[WRITE_SIDE], &msg, 1);
    } else {
        close(fd1[READ_SIDE]);
        close(fd2[WRITE_SIDE]);
        char msg = '1';
        write(fd1[WRITE_SIDE], &msg, 1);
        read(fd2[READ_SIDE], &msg, 1);
        printf("%d: received pong\n", getpid());
    }
    exit();
}