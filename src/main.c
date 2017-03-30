#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "request.h"
#define THREADS 50 // Number of threads to use
#define LOG_PID NULL // When enabled, the current PID will be logged

char const digits[] = "0123456789";
char* pid;
pthread_t thread_pool[THREADS];
static int using_short_pid = NULL;
static int pid_len = 7;
pthread_mutex_t lock;

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
	if (ipid > 9999999 && !using_short_pid) {
		pid_len = 4; // PIDs are only 4 or 7 characters in length
		printf("Tested all the seven digit PIDs. Switching to seven digit.\n");
		using_short_pid = 1;
	}
	while (strlen(pid) < pid_len) {
		prepend(pid, "0");
	}
	return pid;
};

void* thread_callback(void* vargp) {
	pthread_mutex_lock(&lock);
	pid = get_pid(pid);
	pthread_mutex_unlock(&lock);

	if (LOG_PID) printf("Trying PID %s\n", pid);
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
	if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Failed to create mutex lock, exiting..\n");
        return 1;
    }
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