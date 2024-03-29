#!/bin/sh


set -e


GIT_REPO_DIR=$(git rev-parse --show-toplevel)
REL_EXERCISE_DIR="Z2/Z2.2"
CONTAINER_NAME_PREFIX="z41_z_2_2"
SCP_DEST_DIR="~/PSI/lab/Z2/Z2.2"
SERVER_LANG="python"
CLIENT_LANG="c"


BLUE='\033[0;34m'
NC='\033[0m'
INFO_LABEL="${BLUE}INFO: ${NC}"

echo "${INFO_LABEL}GIT_REPO_DIR: $GIT_REPO_DIR"
echo "${INFO_LABEL}REL_EXERCISE_DIR: $REL_EXERCISE_DIR"
echo "${INFO_LABEL}CONTAINER_NAME_PREFIX: $CONTAINER_NAME_PREFIX"
echo "${INFO_LABEL}SCP_DEST_DIR: $SCP_DEST_DIR"
echo "${INFO_LABEL}SERVER_LANG: $SERVER_LANG"
echo "${INFO_LABEL}CLIENT_LANG: $CLIENT_LANG"


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
