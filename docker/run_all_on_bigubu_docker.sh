#!/bin/bash

# This script works if you setup the server to use your public SSH key.
# How to do that:
# - Add your SSH public key to the server using ssh-copy-id.
# - In ~/.ssh/config link the domain to your username and private key.
# Running this script with sudo will most likely require
# a password, as the keys are set up for the user, not root.


set -e


GIT_REPO_DIR=$1             # absolute path to the repo dir
REL_EXERCISE_DIR=$2         # something like Z2/Z2.1
CONTAINER_NAME_PREFIX=$3    # e.g. z41_z_2_1
SCP_DEST_DIR=$4             # e.g. ~/PSI/lab/Z2/Z2.1
SERVER_LANG=$5              # e.g. c
CLIENT_LANG=$6              # e.g. python


CURRENT_SCRIPT_DIR=$(dirname $0)
DOCKER_SCRIPTS_DIR=$CURRENT_SCRIPT_DIR
SERVER_CONTAINER_NAME="${CONTAINER_NAME_PREFIX}_server"
CLIENT_CONTAINER_NAME="${CONTAINER_NAME_PREFIX}_client"


SERVER_EXERCISE_DIR="$GIT_REPO_DIR/$SERVER_LANG/$REL_EXERCISE_DIR"
case $SERVER_LANG in
    "c")
        "$CURRENT_SCRIPT_DIR/server/$SERVER_LANG/run_server_on_bigubu_docker.sh" \
            "$DOCKER_SCRIPTS_DIR" \
            "$SERVER_EXERCISE_DIR" \
            "$SERVER_CONTAINER_NAME" \
            "$SCP_DEST_DIR"
        ;;
    "python")
        "$CURRENT_SCRIPT_DIR/server/$SERVER_LANG/run_server_on_bigubu_docker.sh" \
            "$DOCKER_SCRIPTS_DIR" \
            "$SERVER_EXERCISE_DIR" \
            "$SERVER_CONTAINER_NAME" \
            "$SCP_DEST_DIR"
        ;;
    *)
        echo "ERROR: INVALID SERVER LANGUAGE"
        ;;
esac


CLIENT_EXERCISE_DIR="$GIT_REPO_DIR/$CLIENT_LANG/$REL_EXERCISE_DIR"
case $CLIENT_LANG in
    "c")
        "$CURRENT_SCRIPT_DIR/client/$CLIENT_LANG/run_client_on_bigubu_docker.sh" \
            "$DOCKER_SCRIPTS_DIR" \
            "$CLIENT_EXERCISE_DIR" \
            "$CLIENT_CONTAINER_NAME" \
            "$SERVER_CONTAINER_NAME" \
            "$SCP_DEST_DIR"
        ;;
    *)
        echo "ERROR: INVALID CLIENT LANGUAGE"
        ;;
esac