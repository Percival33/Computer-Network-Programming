all: client server

client: client.c
	gcc -o client client.c -g -lpthread

server: server.c
	gcc -o server server.c -g -lpthread

clean:
	rm client server