
CC=gcc

FLAGS=-g -pthread

all: client.o server.o list-test.o
	$(CC) $(FLAGS) -o chat-client client.o
	$(CC) $(FLAGS) -o chat-server server.o list.o
	$(CC) $(FLAGS) -o list-test list-test.o list.o

client.o: client.c client.h
	$(CC) $(FLAGS) -c client.c client.h

server.o: server.c server.h list.h
	$(CC) $(FLAGS) -c server.c server.h

list-test.o: list-test.c list.o
	$(CC) $(FLAGS) -c list-test.c

list.o: list.c list.h
	$(CC) $(FLAGS) -c list.c list.h

clean:
	rm *.o
	rm *.gch