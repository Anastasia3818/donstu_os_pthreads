#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N 4
#define TOTAL 20

static int buf[N];
static int head = 0;
static int tail = 0;
static int count = 0;

static pthread_mutex_t m        = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  not_full = PTHREAD_COND_INITIALIZER;
static pthread_cond_t  not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    (void)arg;
    for (int i = 1; i <= TOTAL; ++i) {
        pthread_mutex_lock(&m);
        while (count == N) {
            pthread_cond_wait(&not_full, &m);
        }
        buf[head] = i;
        head = (head + 1) % N;
        count++;
        printf("produced: %d (count=%d)\n", i, count);
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&m);
        usleep(50 * 1000);
    }
    return NULL;
}

void *consumer(void *arg) {
    (void)arg;
    for (int i = 1; i <= TOTAL; ++i) {
        pthread_mutex_lock(&m);
        while (count == 0) {
            pthread_cond_wait(&not_empty, &m);
        }
        int v = buf[tail];
        tail = (tail + 1) % N;
        count--;
        printf("consumed: %d (count=%d)\n", v, count);
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&m);
        usleep(80 * 1000);
    }
    return NULL;
}

int main(void) {
    pthread_t pt, ct;
    pthread_create(&pt, NULL, producer, NULL);
    pthread_create(&ct, NULL, consumer, NULL);
    pthread_join(pt, NULL);
    pthread_join(ct, NULL);
    return 0;
}
