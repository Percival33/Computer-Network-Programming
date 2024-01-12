#!/bin/bash

# This script works if you setup the server to use your public SSH key.
# How to do that:
# - Add your SSH public key to the server using ssh-copy-id.
# - In ~/.ssh/config link the domain to your username and private key.
# Running this script with sudo will most likely require
# a password, as the keys are set up for the user, not root.

set -e

SCP_DEST_DIR="~/PSI/projekt/server"

SERVER_ADDRESS="bigubu.ii.pw.edu.pl"
PROJECT_TOP_DIR=$(realpath ../..)/project

echo "You will be connecting to $SERVER_ADDRESS by SSH."

read -p "Username: " USERNAME

echo $(ls $PROJECT_TOP_DIR)

ssh $USERNAME@$SERVER_ADDRESS << EOF
    if [ ! -d $SCP_DEST_DIR ]; then
        mkdir -p $SCP_DEST_DIR
    fi
EOF

scp -r \
    $PROJECT_TOP_DIR \
    $USERNAME@$SERVER_ADDRESS:$SCP_DEST_DIR

ssh $USERNAME@$SERVER_ADDRESS << EOF
    cd $SCP_DEST_DIR/project
    chmod +x main.py

    cd $SCP_DEST_DIR/project/docker
    chmod +x run_on_docker.sh
    chmod +x run_server.py
    sed -i -e 's/\r$//' run_on_docker.sh
    sed -i -e 's/\r$//' run_server.sh
    ./run_on_docker.sh
EOF
