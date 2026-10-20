#include <stdio.h>
#include <pthread.h>

void *threadFunction(void *arg) {
    printf("Thread ID: %lu\n",
           (unsigned long)pthread_self());
    return NULL;
}

int main() {
    pthread_t threads[3];

    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL,
                           threadFunction, NULL) != 0) {
            printf("Thread creation failed\n");
            return 1;
        }
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

