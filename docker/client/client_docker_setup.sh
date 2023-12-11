#!/bin/bash


set -e


CLIENT_CONTAINER_NAME=$1
SERVER_CONTAINER_NAME=$2
LANG=$3


CURRENT_DIR=$(dirname $0)
CLIENT_CONTAINER_TAG="$CLIENT_CONTAINER_NAME:v1"
NETWORK_NAME='z41_network'


# Stop if running
if [[ "$(docker ps | grep $CLIENT_CONTAINER_NAME)" ]]; then
    docker stop $CLIENT_CONTAINER_NAME
fi


# Remove the container
if [[ "$(docker ps -aqf name=$CLIENT_CONTAINER_NAME)" ]]; then
    docker rm $CLIENT_CONTAINER_NAME
fi


docker build \
    -f "$CURRENT_DIR/$LANG/Dockerfile" \
    --tag $CLIENT_CONTAINER_TAG .


SERVER_IP=$(docker inspect -f \
    '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' \
    $SERVER_CONTAINER_NAME)
echo $SERVER_IP


docker run -d \
    --name $CLIENT_CONTAINER_NAME \
    --network $NETWORK_NAME \
    --cap-add NET_ADMIN \
    --env SERVER_IP="$SERVER_IP" \
    $CLIENT_CONTAINER_TAG