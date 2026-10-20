#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

static inline uint64_t rdtsc(void) {
    unsigned int low, high;

    __asm__ volatile (
        "lfence\n\t"
        "rdtsc"
        : "=a"(low), "=d"(high)
        :
        : "memory"
    );

    return ((uint64_t)high << 32) | low;
}

int main() {
    uint64_t start, end;
    pid_t ppid;

    start = rdtsc();

    for (int i = 0; i < 100; i++) {
        ppid = getppid();
    }

    end = rdtsc();

    printf("Parent process ID: %d\n", ppid);
    printf("Number of getppid() calls: 100\n");
    printf("Timestamp counter difference: %llu ticks\n",
           (unsigned long long)(end - start));
    printf("Average ticks per call: %.2f\n",
           (double)(end - start) / 100.0);

    return 0;
}
