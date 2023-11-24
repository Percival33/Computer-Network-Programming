all: client server

client: client.c common.h
	gcc -o client client.c common.c -g -lpthread

server: server.c common.h
	gcc -o server server.c common.c -g -lpthread

clean:
	rm client server