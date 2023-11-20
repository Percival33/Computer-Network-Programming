FROM gcc:latest

WORKDIR /server

COPY ./server.c .
COPY ./Makefile .
COPY ./server_container_config.sh .
RUN chmod +x ./server_container_config.sh
RUN make server

CMD ["./server_container_config.sh"]