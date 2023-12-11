#!/bin/bash

# This script works if you setup the server to use your public SSH key.
# How to do that:
# - Add your SSH public key to the server using ssh-copy-id.
# - In ~/.ssh/config link the domain to your username and private key.
# Running this script with sudo will most likely require
# a password, as the keys are set up for the user, not root.

set -e


EXERCISE_DIR=$1     # e.g. ../../../Z2/Z2.1
CONTAINER_NAME=$2   # e.g. z41_z_2_1_server
SCP_DEST_DIR=$3     # e.g. ~/PSI/lab/Z2/Z2.1


SERVER_ADDRESS="bigubu.ii.pw.edu.pl"
# EXERCISE_DIR might not be absolute
ABS_EXERCISE_DIR=$(realpath $REL_EXERCISE_DIR)


cd $ABS_EXERCISE_DIR/build
cmake ..
make clean client -j$(nproc)


echo "You will be connecting to $SERVER_ADDRESS by SSH."
ssh $SERVER_ADDRESS << EOF
    if [ ! -d $SCP_DEST_DIR/client ]; then
        mkdir -p $SCP_DEST_DIR/client
    fi
EOF


scp $ABS_EXERCISE_DIR/build/output/client \
    $ABS_EXERCISE_DIR/docker/client/Dockerfile \
    $ABS_EXERCISE_DIR/docker/client/client_docker_setup.sh \
    $ABS_EXERCISE_DIR/docker/client/run_client.sh \
    $SERVER_ADDRESS:$SCP_DEST_DIR/client


ssh $SERVER_ADDRESS << EOF
    cd $SCP_DEST_DIR/client
    chmod +x client_docker_setup.sh
    sed -i -e 's/\r$//' client_docker_setup.sh
    sed -i -e 's/\r$//' run_client.sh
    ./client_docker_setup.sh
EOF