#!/bin/bash


set -e


SERVER_CONTAINER_NAME=$1
LANG=$2


CURRENT_DIR=$(dirname $0)
SERVER_CONTAINER_TAG="$SERVER_CONTAINER_NAME:v1"
NETWORK_NAME='z41_network'


if [[ "$(docker ps | grep $SERVER_CONTAINER_NAME)" ]]; then
    docker stop $SERVER_CONTAINER_NAME
fi


if [[ "$(docker ps -aqf name=$SERVER_CONTAINER_NAME)" ]]; then
    docker rm $SERVER_CONTAINER_NAME
fi


docker build \
    -f "$CURRENT_DIR/$LANG/Dockerfile" \
    --tag $SERVER_CONTAINER_TAG .


docker run -d \
    --name $SERVER_CONTAINER_NAME \
    --network $NETWORK_NAME \
    --cap-add NET_ADMIN \
    $SERVER_CONTAINER_TAG
