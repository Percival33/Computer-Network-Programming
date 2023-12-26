FROM ubuntu:22.04

WORKDIR /client

COPY ./client .
COPY ./run_client.sh .
RUN chmod +x ./run_client.sh

# RUN apt-get update
# RUN apt-get install -y netcat
# RUN apt-get install -y iputils-ping
# RUN apt-get install -y iproute2

CMD ["sleep", "infinity"]
