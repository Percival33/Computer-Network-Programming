FROM gcc:latest

WORKDIR /server

RUN apt-get update
RUN apt-get install -y iproute2

COPY ./server.c .
COPY ./Makefile .
COPY ./server_container_config.sh .
RUN chmod +x ./server_container_config.sh
RUN make server

CMD ["./server_container_config.sh"]