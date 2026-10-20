#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void print_limit(const char *name, int resource) {
    struct rlimit limit;

    if (getrlimit(resource, &limit) == -1) {
        perror("getrlimit");
        return;
    }

    printf("%s\n", name);

    if (limit.rlim_cur == RLIM_INFINITY)
        printf("  Soft limit: Unlimited\n");
    else
        printf("  Soft limit: %llu\n",
               (unsigned long long)limit.rlim_cur);

    if (limit.rlim_max == RLIM_INFINITY)
        printf("  Hard limit: Unlimited\n");
    else
        printf("  Hard limit: %llu\n",
               (unsigned long long)limit.rlim_max);

    printf("\n");
}

int main() {
    print_limit("CPU time:", RLIMIT_CPU);
    print_limit("Maximum file size:", RLIMIT_FSIZE);
    print_limit("Number of open files:", RLIMIT_NOFILE);
    print_limit("Maximum processes:", RLIMIT_NPROC);
    print_limit("Stack size:", RLIMIT_STACK);
    print_limit("Core file size:", RLIMIT_CORE);
    print_limit("Virtual memory:", RLIMIT_AS);

    return 0;
}
