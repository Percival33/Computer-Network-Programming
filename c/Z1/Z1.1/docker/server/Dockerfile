FROM ubuntu:22.04

WORKDIR /server

COPY ./server .
COPY ./server_container_config.sh .
RUN chmod 777 ./server_container_config.sh

# RUN apt-get update
# RUN apt-get install -y netcat
# RUN apt-get install -y iputils-ping
# RUN apt-get install -y iproute2

CMD ["sleep", "infinity"]
