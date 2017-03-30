#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "request.h"
#define THREADS 50 // Number of threads to use
#define LOG_PID NULL // When enabled, the current PID will be logged

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
	int pid_len = 4;
	if (ipid > 9999) {
		pid_len = 7; // PIDs are only 4 or 7 characters in length
	}
	while (strlen(pid) < pid_len) {
		prepend(pid, "0");
	}
	return pid;
};
char* pid;
pthread_t thread_pool[THREADS];

void* thread_callback(void* vargp) {
	pid = get_pid(pid);
	if (LOG_PID) printf("%s\n", pid);
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
	setup_openssl();
	printf("Starting PID bruteforce...\n");
	pid = malloc(9);
	strcpy(pid, "0000000");
	int tthread = 0;
	for (; tthread < THREADS; ++tthread) {
		pthread_create(&thread_pool[tthread], NULL, &thread_callback, NULL);
	}
	for (tthread = 0; tthread < THREADS; ++tthread) {
		printf("Starting thread %d\n", tthread);
		pthread_detach(thread_pool[tthread]);
	}
	printf("Press enter to terminate\n");
	getchar();
};