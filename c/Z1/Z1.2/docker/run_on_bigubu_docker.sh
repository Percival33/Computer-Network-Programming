#!/bin/bash

# This script works if you setup the server to use your public SSH key.
# How to do that:
# - Add your SSH public key to the server using ssh-copy-id.
# - In ~/.ssh/config link the domain to your username and private key.
# Running this script with sudo will most likely require
# a password, as the keys are set up for the user, not root.

set -e

DEFAULT_CONTAINER_NAME="z41_z_1_2_c_server"
DEFAULT_SCP_DEST_DIR="~/PSI/lab_1/Z1.2"

CONTAINER_NAME=${1:-$DEFAULT_CONTAINER_NAME}
SCP_DEST_DIR=${2:-$DEFAULT_SCP_DEST_DIR}

SERVER_ADDRESS="bigubu.ii.pw.edu.pl"
EXERCISE_DIR="$PWD/../../../Z1"

cd $EXERCISE_DIR/Z1.2/build
cmake ..
make clean server client -j$(nproc)

echo "You will be connecting to $SERVER_ADDRESS by SSH."

# read -p "Username: " USERNAME

# This also overrides the previous files
# scp -r $EXERCISE_DIR $SERVER_ADDRESS:$SCP_DEST_DIR

ssh $SERVER_ADDRESS << EOF
    if [ ! -d $SCP_DEST_DIR/server ]; then
        mkdir -p $SCP_DEST_DIR/server
    fi
    if [ ! -d $SCP_DEST_DIR/client ]; then
        mkdir -p $SCP_DEST_DIR/client
    fi
EOF

scp $EXERCISE_DIR/Z1.2/build/output/server \
    $EXERCISE_DIR/Z1.2/docker/server/Dockerfile \
    $EXERCISE_DIR/Z1.2/docker/server/docker_c_server_startup.sh \
    $EXERCISE_DIR/Z1.2/docker/server/server_container_config.sh \
    $SERVER_ADDRESS:$SCP_DEST_DIR/server

scp $EXERCISE_DIR/Z1.2/build/output/client \
    $EXERCISE_DIR/Z1.2/docker/client/Dockerfile \
    $EXERCISE_DIR/Z1.2/docker/client/client_container_config.sh \
    $EXERCISE_DIR/Z1.2/docker/client/docker_c_client_startup.sh \
    $USERNAME@$SERVER_ADDRESS:$SCP_DEST_DIR/client

ssh $SERVER_ADDRESS << EOF
    cd $SCP_DEST_DIR/server
    chmod +x docker_c_server_startup.sh
    sed -i -e 's/\r$//' server_container_config.sh
    sed -i -e 's/\r$//' docker_c_server_startup.sh
    ./docker_c_server_startup.sh

    cd $SCP_DEST_DIR/client
    chmod +x docker_c_client_startup.sh
    sed -i -e 's/\r$//' client_container_config.sh
    sed -i -e 's/\r$//' docker_c_client_startup.sh
    ./docker_c_client_startup.sh
EOF
