#!/bin/bash

set -e

PROJECT_TOP_DIR=$(realpath ../../..)/project

cd $PROJECT_TOP_DIR
python3 ./main.py
