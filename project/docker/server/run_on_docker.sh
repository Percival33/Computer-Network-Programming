#!/bin/bash

set -e

PROJECT_TOP_DIR=$(realpath ../../..)/project
CONTAINER_NAME='z41_project_server'
SCRIPT_DIR=$(dirname $0)

if [[ "$(docker ps | grep $CONTAINER_NAME)" ]]; then
    docker stop $CONTAINER_NAME
fi

if [[ "$(docker ps -aqf name=$CONTAINER_NAME)" ]]; then
    docker rm $CONTAINER_NAME
fi

# Build from the top dir
cd $PROJECT_TOP_DIR
docker build --tag $CONTAINER_NAME:v1 ./docker/server

docker run -d \
    --name $CONTAINER_NAME \
    --network z41_network \
    --cap-add NET_ADMIN \
    $CONTAINER_NAME:v1
