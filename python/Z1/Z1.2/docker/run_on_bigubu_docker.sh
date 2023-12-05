#!/bin/bash

# This script works if you setup the server to use your public SSH key.
# How to do that:
# - Add your SSH public key to the server using ssh-copy-id.
# - In ~/.ssh/config link the domain to your username and private key.
# Running this script with sudo will most likely require
# a password, as the keys are set up for the user, not root.

set -e

DEFAULT_CONTAINER_NAME="z41_z_1_2_python_server"
DEFAULT_SCP_DEST_DIR="~/PSI/lab_1/Z1.2"

CONTAINER_NAME=${1:-$DEFAULT_CONTAINER_NAME}
SCP_DEST_DIR=${2:-$DEFAULT_SCP_DEST_DIR}

SERVER_ADDRESS="bigubu.ii.pw.edu.pl"
EXERCISE_DIR="$PWD/../../../Z1"

echo "You will be connecting to $SERVER_ADDRESS by SSH."

read -p "Username: " USERNAME

# This also overrides the previous files
# scp -r $EXERCISE_DIR $SERVER_ADDRESS:$SCP_DEST_DIR

ssh $USERNAME@$SERVER_ADDRESS << EOF
    if [ ! -d $SCP_DEST_DIR/server ]; then
        mkdir -p $SCP_DEST_DIR/server
    fi
    if [ ! -d $SCP_DEST_DIR/client ]; then
        mkdir -p $SCP_DEST_DIR/client
    fi
EOF

scp $EXERCISE_DIR/Z1.2/server.py \
    $EXERCISE_DIR/Z1.2/docker/server/Dockerfile \
    $EXERCISE_DIR/Z1.2/docker/server/docker_python_server_startup.sh \
    $EXERCISE_DIR/Z1.2/docker/server/server_container_config.sh \
    $USERNAME@$SERVER_ADDRESS:$SCP_DEST_DIR/server

scp $EXERCISE_DIR/Z1.2/client.py \
    $EXERCISE_DIR/Z1.2/docker/client/Dockerfile \
    $EXERCISE_DIR/Z1.2/docker/client/client_container_config.sh \
    $EXERCISE_DIR/Z1.2/docker/client/docker_python_client_startup.sh \
    $USERNAME@$SERVER_ADDRESS:$SCP_DEST_DIR/client

ssh $USERNAME@$SERVER_ADDRESS << EOF
    cd $SCP_DEST_DIR/server
    chmod +x docker_python_server_startup.sh
    chmod +x server.py
    sed -i -e 's/\r$//' server_container_config.sh
    sed -i -e 's/\r$//' docker_python_server_startup.sh
    ./docker_python_server_startup.sh

    cd $SCP_DEST_DIR/client
    chmod +x docker_python_client_startup.sh
    chmod +x client.py
    sed -i -e 's/\r$//' client_container_config.sh
    sed -i -e 's/\r$//' docker_python_client_startup.sh
    ./docker_python_client_startup.sh
EOF
 