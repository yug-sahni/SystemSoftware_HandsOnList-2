#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/sysinfo.h>

int main() {
    long value;
    struct sysinfo info;

    value = sysconf(_SC_ARG_MAX);
    printf("a. Maximum argument size for exec: %ld bytes\n", value);

    value = sysconf(_SC_CHILD_MAX);
    printf("b. Maximum simultaneous processes per user ID: %ld\n",
           value);

    value = sysconf(_SC_CLK_TCK);
    printf("c. Clock ticks per second: %ld\n", value);

    value = sysconf(_SC_OPEN_MAX);
    printf("d. Maximum open files per process: %ld\n", value);

    value = sysconf(_SC_PAGESIZE);
    printf("e. Page size: %ld bytes\n", value);

    if (sysinfo(&info) == 0) {
        printf("f. Total physical memory pages: %llu\n",
               (unsigned long long)
               ((unsigned long long)info.totalram * info.mem_unit / value));

        printf("g. Available physical memory pages: %llu\n",
               (unsigned long long)
               ((unsigned long long)info.freeram * info.mem_unit / value));
    } else {
        perror("sysinfo");
        return 1;
    }

    return 0;
}
