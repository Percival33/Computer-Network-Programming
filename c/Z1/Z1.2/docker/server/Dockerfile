FROM ubuntu:22.04

WORKDIR /server

RUN apt-get update
RUN apt-get install -y iproute2

COPY ./server .
COPY ./server_container_config.sh .
RUN chmod 777 ./server_container_config.sh

RUN apt-get update && apt-get install -y netcat
EXPOSE 8888/udp
EXPOSE 8889/udp

CMD ["/bin/sh", "./server_container_config.sh"]