#!/bin/bash


# This script works if you setup the server to use your public SSH key.
# How to do that:
# - Add your SSH public key to the server using ssh-copy-id.
# - In ~/.ssh/config link the domain to your username and private key.
# Running this script with sudo will most likely require
# a password, as the keys are set up for the user, not root.


set -e


echo "C client setup..."


DOCKER_SCRIPTS_DIR=$1 
EXERCISE_DIR=$2             # something like .../Z2/Z2.1
CLIENT_CONTAINER_NAME=$3    # e.g. z41_z_2_1_server
SERVER_CONTAINER_NAME=$4    # e.g. z41_z_2_1_server
SCP_DEST_DIR=$5             # e.g. ~/PSI/lab/Z2/Z2.1


SERVER_ADDRESS="bigubu.ii.pw.edu.pl"


cd $EXERCISE_DIR/build
cmake ..
make clean client -j$(nproc)


echo "You will be connecting to $SERVER_ADDRESS by SSH."
ssh $SERVER_ADDRESS << EOF
    if [ ! -d $SCP_DEST_DIR/client ]; then
        mkdir -p $SCP_DEST_DIR/client
    fi
EOF


scp $EXERCISE_DIR/build/output/client \
    $DOCKER_SCRIPTS_DIR/client/c/Dockerfile \
    $DOCKER_SCRIPTS_DIR/client/c/run_client.sh \
    $DOCKER_SCRIPTS_DIR/client/client_docker_setup.sh \
    $SERVER_ADDRESS:$SCP_DEST_DIR/client


ssh $SERVER_ADDRESS << EOF
    cd $SCP_DEST_DIR/client
    chmod +x client_docker_setup.sh
    sed -i -e 's/\r$//' client_docker_setup.sh
    sed -i -e 's/\r$//' run_client.sh
    ./client_docker_setup.sh $CLIENT_CONTAINER_NAME $SERVER_CONTAINER_NAME
EOF