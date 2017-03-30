#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "request.h"
#define THREADS 10

char const digits[] = "0123456789";
void prepend(char* s, const char* t) {
    size_t len = strlen(t);
    size_t i;
    memmove(s + len, s, strlen(s) + 1);
    for (i = 0; i < len; ++i) {
        s[i] = t[i];
    }
};
char* get_pid(char* pid) {
	int ipid = atoi(pid);
	++ipid;
	sprintf(pid, "%d", ipid);
	while (strlen(pid) < 4) {
		prepend(pid, "0");
	}
	return pid;
};
char* pid;
pthread_t thread_pool[25];

void* thread_callback(void* vargp) {
	pid = get_pid(pid);
	printf("%s\n", pid);
	req* r = init_request();
	char* res = request_kahoot_token(r, pid);
	if (res != NULL) {
		printf("Discovered Kahoot PID: %s\n", pid);
	}
	request_free(r);
	free(res);
	thread_callback(vargp);
};
int main(int argc, char* argv[]) {
	pid = malloc(9);
	strcpy(pid, "0000000");
	int pool_size = sizeof(pthread_t) * THREADS;
	int tthread = 0;
	while (tthread < pool_size) {
		pthread_create(&thread_pool[tthread], NULL, &thread_callback, NULL);
		pthread_join(thread_pool[tthread], NULL);
		tthread++;
	}
	getchar();
};