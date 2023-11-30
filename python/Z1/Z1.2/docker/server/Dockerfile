# FROM ubuntu:22.04
FROM python:3.10

WORKDIR /server

RUN apt-get update
RUN apt-get install -y iproute2

# COPY ./server .
# COPY ./server_container_config.sh .
COPY ../server.py .
COPY server_container_config.sh .

RUN chmod 777 ./server_container_config.sh

RUN apt-get update && apt-get install -y netcat-openbsd
RUN apt-get install -y python3

EXPOSE 8888/udp
EXPOSE 8889/udp

CMD ["/bin/sh", "./server_container_config.sh"]