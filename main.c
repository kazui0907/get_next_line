#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <unistd.h>
#include "get_next_line.h"

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q a.out");
// }

int main(void)
{
    int fd;
    char *line;

    fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return 1;
    }

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd);
    return 0;
}
