FROM gcc:latest

WORKDIR .

RUN mkdir /server
RUN cd /server
COPY ./server.c .
COPY ./Makefile .
RUN make server
RUN ./server 8888