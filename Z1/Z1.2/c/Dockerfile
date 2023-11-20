FROM gcc:latest

WORKDIR /server

COPY ./server.c .
COPY ./Makefile .
RUN make server

CMD ["./server", "8888"]