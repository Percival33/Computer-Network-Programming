#!/bin/sh


set -e


GIT_REPO_DIR=$(realpath "../../..")
REL_EXERCISE_DIR="Z2/Z2.2"
CONTAINER_NAME_PREFIX="z41_z_2_2"
SCP_DEST_DIR="~/PSI/lab/Z2/Z2.2"
SERVER_LANG="python"
CLIENT_LANG="c"


DOCKER_SCRIPTS_DIR="$GIT_REPO_DIR/docker"


$DOCKER_SCRIPTS_DIR/run_all_on_bigubu_docker.sh \
    $GIT_REPO_DIR \
    $REL_EXERCISE_DIR \
    $CONTAINER_NAME_PREFIX \
    $SCP_DEST_DIR \
    $SERVER_LANG \
    $CLIENT_LANG