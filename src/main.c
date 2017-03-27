#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "request.h"

char const digits[] = "0123456789";
char* itoa(int i, char b[]){
    char* p = b;
    if(i<0) {
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do {
        ++p;
        shifter = shifter/10;
    } while(shifter);
    *p = '\0';
    do {
        *--p = digits[i%10];
        i = i/10;
    } while(i);
    return b;
};
char* pid = "0000000";

int main(int argc, char* argv[]) {
	//
};