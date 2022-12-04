#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "get_num.h"

int usage()
{
    char msg[] = "Usage: samle [-a] filename total_write_size";
    fprintf(stderr, "%s\n", msg);

    return 0;
}

int main(int argc, char *argv[])
{
    int do_append = 0;
    int c;
    while ( (c = getopt(argc, argv, "a")) != -1) {
        switch (c) {
            case 'a':
                do_append = 1;
                break;
            default:
                break;
        }
    }
    argc -= optind;
    argv += optind;
    if (argc != 2) {
        usage();
        exit(1);
    }

    char *filename = argv[0];
    int total_write_size = get_num(argv[1]);

    int open_flags = O_CREAT|O_WRONLY;
    if (do_append) {
        open_flags = (open_flags | O_APPEND);
    }
    int fd = open(filename, open_flags, 0644);
    
    char buf[32*1024];
    memset(buf, 'X', sizeof(buf));
    
    int write_count = total_write_size / (32*1024);

    for (int i = 0; i < write_count; ++i) {
        int n = write(fd, buf, sizeof(buf));
        if (n < 0) {
            err(1, "write");
        }
        struct stat statbuf;
        if (stat(filename, &statbuf) < 0) {
            err(1, "stat");
        }
        printf("%ld %ld\n", statbuf.st_size, statbuf.st_blocks*512);
    }

    return 0;
}
