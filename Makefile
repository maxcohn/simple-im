
CC=gcc

FLAGS=-g -pthread

all: client.o server.o list-test.o user.o
	$(CC) $(FLAGS) -o chat-client client.o
	$(CC) $(FLAGS) -o chat-server server.o list.o user.o
	$(CC) $(FLAGS) -o list-test list-test.o list.o

client.o: client/client.c client/client.h
	$(CC) $(FLAGS) -c client/client.c client/client.h

server.o: server/server.c server/server.h utils/list.h
	$(CC) $(FLAGS) -c server/server.c server/server.h

list-test.o: utils/list-test.c list.o
	$(CC) $(FLAGS) -c utils/list-test.c

list.o: utils/list.c utils/list.h
	$(CC) $(FLAGS) -c utils/list.c utils/list.h

user.o: utils/user.c utils/user.h
	$(CC) $(FLAGS) -c utils/user.c utils/user.h

clean:
	rm *.o
	rm *.gch
	rm chat-client
	rm chat-server
	rm list-test