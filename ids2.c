#include <pthread.h>
#include <stdio.h>

void *worker(void *arg) {
    (void)arg;

    pthread_t self = pthread_self();
    pthread_t other = *(pthread_t *)arg;   /* pthread_t, переданный из main */

    if (pthread_equal(self, other)) {
        printf("worker: self == other (pthread_equal говорит ДА)\n");
    } else {
        printf("worker: self != other (pthread_equal говорит НЕТ)\n");
    }

    /* Попытка сравнить через == (в glibc скомпилируется, но так делать НЕЛЬЗЯ) */
    /* if (self == other) { ... } -- стандарт POSIX не гарантирует, что это сработает */

    return NULL;
}

int main(void) {
    pthread_t t;
    pthread_create(&t, NULL, worker, &t);
    pthread_join(t, NULL);

    return 0;
}
