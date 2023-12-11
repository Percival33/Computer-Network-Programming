#!/bin/sh


set -e


DOCKER_SCRIPTS_DIR="$PWD/../../../docker"


$DOCKER_SCRIPTS_DIR/run_all_on_bigubu_docker.sh \
    ../../../Z2/Z2.1 \
    z41_z_2_1_server \
    

