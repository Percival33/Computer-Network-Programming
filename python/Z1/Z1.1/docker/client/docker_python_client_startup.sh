#!/bin/bash

set -e

CLIENT_CONTAINER_NAME='z41_z_1_1_python_client'
SERVER_CONTAINER_NAME='z41_z_1_1_python_server'

# Stop if running
if [[ "$(docker ps | grep $CLIENT_CONTAINER_NAME)" ]]; then
    docker stop $CLIENT_CONTAINER_NAME
fi

# Remove the container
if [[ "$(docker ps -aqf name=$CLIENT_CONTAINER_NAME)" ]]; then
    docker rm $CLIENT_CONTAINER_NAME
fi

docker build --tag $CLIENT_CONTAINER_NAME:v1 .

SERVER_IP=$(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' $SERVER_CONTAINER_NAME)
echo $SERVER_IP

docker run -d \
    --name $CLIENT_CONTAINER_NAME \
    --network z41_network \
    --cap-add NET_ADMIN \
    --env SERVER_IP="$SERVER_IP" \
    $CLIENT_CONTAINER_NAME:v1
