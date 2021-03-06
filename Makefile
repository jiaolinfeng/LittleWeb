CC = g++
FLAGS = -Wall -ansi -g 

all: main 
main: server common
	$(CC) $(FLAGS) -o start start.cpp -lpthread thread.o poller.o server.o http.o
server: server.h server.cpp  
	$(CC) $(FLAGS) -c server.cpp 
common: thread.h thread.cpp common.h poller.h poller.cpp http.h http.cpp
	$(CC) $(FLAGS) -c thread.cpp poller.cpp http.cpp
	


