#!/bin/bash

set -e

CONTAINER_NAME='z41_z_11_python_server'

if [[ "$(docker ps | grep $CONTAINER_NAME)" ]]; then
    docker stop $CONTAINER_NAME
fi

if [[ "$(docker ps -aqf name=$CONTAINER_NAME)" ]]; then
    docker rm $CONTAINER_NAME
fi

docker build --tag $CONTAINER_NAME:v1 .

docker run -d \
    --name $CONTAINER_NAME \
    --network z41_network \
    --cap-add NET_ADMIN \
    $CONTAINER_NAME:v1