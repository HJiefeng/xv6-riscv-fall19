#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char *argv[])
{  
    if (argc != 2) 
    {
        printf("sleep $(time)\n");
        exit();
    }
    sleep(atoi(argv[1]));
    exit();
}