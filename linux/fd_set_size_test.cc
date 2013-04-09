
#define FD_SETSIZE  1024*1024    //这种方式在linux上doesn't work

#include <sys/select.h>
#include <stdio.h>

int main(int argc, const char *argv[])
{

    printf("fs_set size:%d\n", sizeof(fd_set));

    return 0;
}

