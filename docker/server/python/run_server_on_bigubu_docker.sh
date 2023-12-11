#!/bin/bash

# This script works if you setup the server to use your public SSH key.
# How to do that:
# - Add your SSH public key to the server using ssh-copy-id.
# - In ~/.ssh/config link the domain to your username and private key.
# Running this script with sudo will most likely require
# a password, as the keys are set up for the user, not root.


set -e


echo "Python server setup..."


DOCKER_SCRIPTS_DIR=$1   
EXERCISE_DIR=$2             # something like .../python/Z2/Z2.1
SERVER_CONTAINER_NAME=$3    # e.g. z41_z_2_1_server
SCP_DEST_DIR=$4             # e.g. ~/PSI/lab/Z2/Z2.1


SERVER_ADDRESS="bigubu.ii.pw.edu.pl"


echo "You will be connecting to $SERVER_ADDRESS by SSH."
ssh $SERVER_ADDRESS << EOF
    if [ ! -d $SCP_DEST_DIR/server ]; then
        mkdir -p $SCP_DEST_DIR/server
    fi
EOF


scp $EXERCISE_DIR/server.py \
    $DOCKER_SCRIPTS_DIR/server/python/Dockerfile \
    $DOCKER_SCRIPTS_DIR/server/python/run_server.sh \
    $DOCKER_SCRIPTS_DIR/server/server_docker_setup.sh \
    $SERVER_ADDRESS:$SCP_DEST_DIR/server


ssh $SERVER_ADDRESS << EOF
    cd $SCP_DEST_DIR/server
    chmod +x server_docker_setup.sh
    sed -i -e 's/\r$//' server_docker_setup.sh
    sed -i -e 's/\r$//' run_server.sh
    ./server_docker_setup.sh $SERVER_CONTAINER_NAME
EOF