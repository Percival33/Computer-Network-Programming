FROM ubuntu:22.04

WORKDIR /server

COPY ./server .
COPY ./run_server.sh .
RUN chmod +x ./run_server.sh

# RUN apt-get update
# RUN apt-get install -y netcat
# RUN apt-get install -y iputils-ping
# RUN apt-get install -y iproute2

CMD ["sleep", "infinity"]