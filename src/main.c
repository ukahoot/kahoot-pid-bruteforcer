#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "request.h"

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

int main(int argc, char* argv[]) {
	//
};