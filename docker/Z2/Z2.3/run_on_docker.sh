#!/bin/sh


set -e


GIT_REPO_DIR=$(realpath "../../..")
REL_EXERCISE_DIR="Z2/Z2.3"
CONTAINER_NAME_PREFIX="z41_z_2_3"
SCP_DEST_DIR="~/PSI/lab/Z2/Z2.3"
SERVER_LANG="c"
CLIENT_LANG="c"


DOCKER_SCRIPTS_DIR="$GIT_REPO_DIR/docker"


DOCKER_RUN_ALL_SCRIPT="$DOCKER_SCRIPTS_DIR/run_all_on_bigubu_docker.sh"
chmod +x $DOCKER_RUN_ALL_SCRIPT
dos2unix $DOCKER_RUN_ALL_SCRIPT
bash $DOCKER_RUN_ALL_SCRIPT \
    $GIT_REPO_DIR \
    $REL_EXERCISE_DIR \
    $CONTAINER_NAME_PREFIX \
    $SCP_DEST_DIR \
    $SERVER_LANG \
    $CLIENT_LANG
