#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

int main() {
    struct rlimit limit;

    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        return 1;
    }

    printf("Old soft limit: %llu\n",
           (unsigned long long)limit.rlim_cur);
    printf("Old hard limit: %llu\n",
           (unsigned long long)limit.rlim_max);

    limit.rlim_cur = 50;

    if (setrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("setrlimit");
        return 1;
    }

    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        return 1;
    }

    printf("\nUpdated soft limit: %llu\n",
           (unsigned long long)limit.rlim_cur);
    printf("Hard limit: %llu\n",
           (unsigned long long)limit.rlim_max);

    return 0;
}
