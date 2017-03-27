all:
	clear
	gcc src/*.c -o out -lssl -lcrypto -pthread -lpthread 