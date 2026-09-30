#include "pthreadfuncs.h"
#include "pthreadfuncs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>


int main(void) {
    // headline
    about();

    // sys call - open
    // file, modes, rights
    g_fd = open("output.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (g_fd < 0) {
        perror("open output.log");
        return 1;
    }
struct ThreadArgs args[NUM_THREADS];
for (int i = 0; i < NUM_THREADS; ++i){
args[i].id = i;
snprintf(args[i].tag, sizeof(args[i].tag), "T%d", i);
snprintf(args[i].message, sizeof(args[i].message),
"Hello from thread %d", i);
}

char msg[128];
snprintf(msg, sizeof(msg), "main: pid = %d\n", (int)getpid());
if (write_line(msg) !=0){
fprintf(stderr, "write_line failed in main\n");
}

pthread_t threads[NUM_THREADS];
for (int i = 0; i < NUM_THREADS; ++i){
int rc = pthread_create(&threads[i], NULL, func_thread, &args[i]);
if (rc !=0){
fprintf(stderr, "pthread_create[%d]: %s\n", i, strerror(rc));
close(g_fd);
return 1;
}
}
for (int i = 0; i < NUM_THREADS; ++i){
void *ret = NULL;
int rc = pthread_join(threads[i], &ret);
if (rc == 0){
if (ret != NULL){
printf("thread[%d] returned: %s\n", i, (char *)ret);
free(ret);
} else {
printf("thread[%d] returned NULL\n", i);
}
}else {
fprintf(stderr, "pthread_join[%d]: rc=%d (%s)\n",
i, rc, strerror(rc));
}
}


close(g_fd);
return 0;
}

