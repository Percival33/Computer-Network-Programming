FROM ubuntu:22.04

WORKDIR /client

# RUN apt-get update
# RUN apt-get install -y iproute2

# COPY ./client .
# COPY ./client_container_config.sh .
COPY ../client.py .
COPY client_container_config.sh .

RUN chmod 777 ./client_container_config.sh

RUN apt-get update && apt-get install -y netcat
# RUN apt-get install -y python3
EXPOSE 8888/udp
EXPOSE 8889/udp

CMD ["/bin/sh", "./client_container_config.sh"]