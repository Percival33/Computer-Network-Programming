#!/bin/bash

set -e

CONTAINER_NAMES=("z1_server" "z1_client")

for CONTAINER_NAME in "${CONTAINER_NAMES[@]}"; do
    echo "Processing container: $CONTAINER_NAME"

    if [[ "$(docker ps | grep $CONTAINER_NAME)" ]]; then
        echo "Stopping container: $CONTAINER_NAME"
        docker stop $CONTAINER_NAME
    fi

    if [[ "$(docker ps -aqf name=$CONTAINER_NAME)" ]]; then
        echo "Removing container: $CONTAINER_NAME"
        docker rm $CONTAINER_NAME
    fi

    if [[ $CONTAINER_NAME == "z1_server" ]]; then
        echo "Building image for container: $CONTAINER_NAME"
        docker build -t z1_server . -f ./Dockerfile.server
    elif [[ $CONTAINER_NAME == "z1_client" ]]; then
        echo "Building image for container: $CONTAINER_NAME"
        docker build -t z1_client . -f ./Dockerfile.client
    fi

    docker run -d \
        --name $CONTAINER_NAME \
        --network psi_network \
        $CONTAINER_NAME
done
