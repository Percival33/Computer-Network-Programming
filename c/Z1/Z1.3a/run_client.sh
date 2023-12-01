#!/bin/bash
# Created by Marcin Jarczewski on 30/11/2023.

# Find the container ID of the z1_server container
CONTAINER_ID=$(docker ps -qf "name=z1_client")

if [ -z "$CONTAINER_ID" ]; then
    echo "No running container found with name z1_client"
    exit 1
fi

# Connect to the container using bash
docker exec -it $CONTAINER_ID bash
