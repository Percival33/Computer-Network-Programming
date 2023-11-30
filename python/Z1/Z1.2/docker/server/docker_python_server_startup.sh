#!/bin/bash

set -e

CONTAINER_NAME='z41_z_1_python_server'

if [[ "$(docker ps | grep $CONTAINER_NAME)" ]]; then
    docker stop $CONTAINER_NAME
fi

if [[ "$(docker ps -aqf name=$CONTAINER_NAME)" ]]; then
    docker rm $CONTAINER_NAME
fi

docker build --tag $CONTAINER_NAME:v1 .

docker create -p 8888:8888/udp -p 8889:8889/udp -it --name $CONTAINER_NAME --network z41_network --cap-add NET_ADMIN $CONTAINER_NAME:v1

docker start -ai $CONTAINER_NAME