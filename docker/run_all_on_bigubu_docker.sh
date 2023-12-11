#!/bin/bash

# This script works if you setup the server to use your public SSH key.
# How to do that:
# - Add your SSH public key to the server using ssh-copy-id.
# - In ~/.ssh/config link the domain to your username and private key.
# Running this script with sudo will most likely require
# a password, as the keys are set up for the user, not root.


set -e


EXERCISE_DIR=$1             # e.g. ../../../Z2/Z2.1
CONTAINER_NAME_PREFIX=$2    # e.g. z41_z_2_1
SCP_DEST_DIR=$3             # e.g. ~/PSI/lab/Z2/Z2.1
SERVER_LANG=$4              # e.g. C
CLIENT_LANG=$5              # e.g. Python


SERVER_ADDRESS="bigubu.ii.pw.edu.pl"
# EXERCISE_DIR might not be absolute
ABS_EXERCISE_DIR=$(realpath $REL_EXERCISE_DIR)
CURRENT_SCRIPT_DIR=$(dirname $0)
SERVER_CONTAINER_NAME="${CONTAINER_NAME_PREFIX}_server"
CLIENT_CONTAINER_NAME="${CONTAINER_NAME_PREFIX}_client"


case $SERVER_LANG in
    "C" | "c")
        $CURRENT_SCRIPT_DIR/server/c/run_c_server_on_bigubu_docker.sh \
            "$EXERCISE_DIR" \
            "$SERVER_CONTAINER_NAME" \
            "$SCP_DEST_DIR"
        ;;
    "Python" | "python")
        $CURRENT_SCRIPT_DIR/server/python/run_python_server_on_bigubu_docker.sh \
            "$EXERCISE_DIR" \
            "$SERVER_CONTAINER_NAME" \
            "$SCP_DEST_DIR"
        ;;
    *)
        echo "ERROR: INVALID SERVER LANGUAGE"
        ;;
esac
case $CLIENT_LANG in
    "C" | "c")
        $CURRENT_SCRIPT_DIR/client/c/run_c_client_on_bigubu_docker.sh \
            "$EXERCISE_DIR" \
            "$CLIENT_CONTAINER_NAME" \
            "$SCP_DEST_DIR"
        ;;
    *)
        echo "ERROR: INVALID CLIENT LANGUAGE"
        ;;
esac