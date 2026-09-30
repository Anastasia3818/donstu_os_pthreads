#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

void *worker(void *arg) {
    (void)arg;
    pid_t   tid = (pid_t) syscall(SYS_gettid);
    pthread_t pt = pthread_self();

    printf("worker: tid = %d, pthread_self = %lu\n",
           (int)tid, (unsigned long)pt);
    return NULL;
}

int main(void) {
    pid_t   tid_main = (pid_t) syscall(SYS_gettid);
    pthread_t pt_main = pthread_self();

    printf("main:   tid = %d, pthread_self = %lu\n",
           (int)tid_main, (unsigned long)pt_main);

    pthread_t t;
    pthread_create(&t, NULL, worker, NULL);
    pthread_join(t, NULL);

    return 0;
}
