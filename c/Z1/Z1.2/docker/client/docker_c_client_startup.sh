#!/bin/bash

set -e

CONTAINER_NAME='z41_z_1_c_client'

if [[ "$(docker ps | grep $CONTAINER_NAME)" ]]; then
    docker stop $CONTAINER_NAME
fi

if [[ "$(docker ps -aqf name=$CONTAINER_NAME)" ]]; then
    docker rm $CONTAINER_NAME
fi

docker build --tag $CONTAINER_NAME:v1 .

docker create -it --name $CONTAINER_NAME --network z41_network --cap-add NET_ADMIN $CONTAINER_NAME:v1

docker start -ai $CONTAINER_NAME