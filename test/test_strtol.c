#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main(void) {
    long a;
    char buf[1024];
    int success;
    do {
        printf("enter a number: ");
        if (!fgets(buf, 1024, stdin)) return 1;
        char *endptr;
        errno = 0;
        a = strtod(buf, &endptr);
        if (errno == ERANGE || endptr == buf || (*endptr && *endptr != '\n'))
            success = 0;
        else
            success = 1;
    } while (!success);
    printf("You entered %ld.\n", a);
}
