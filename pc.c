#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static int buffer = 0;
static int has_item = 0;

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
(void)arg;
for (int i = 1; i <= 5; ++i) {
pthread_mutex_lock(&m);
while (has_item) {
pthread_cond_wait(&c, &m);
}

buffer = i;
has_item = 1;
printf("produced: %d\n", i);

pthread_cond_signal(&c);
pthread_mutex_unlock(&m);

usleep(100 * 1000);
}
return NULL;
}
void *consumer(void *arg) {
(void)arg;
for (int i = 1; i <= 5; ++i){
pthread_mutex_lock(&m);

while (!has_item) {
pthread_cond_wait(&c, &m);
}
printf("consumed: %d\n", buffer);
has_item = 0;
pthread_cond_signal(&c);
pthread_mutex_unlock(&m);

usleep(150 * 1000);
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
