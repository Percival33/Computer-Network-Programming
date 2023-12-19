#!/bin/sh

SEND_BUFFER_SIZE=$1
CLIENT_PROGRAM='./build/output/client'
SERVER_IP='127.0.0.1'
SERVER_PORT=8888
LOGS_DIR="./experiment_2_logs/sndbuf_${SEND_BUFFER_SIZE}kB.txt"

$CLIENT_PROGRAM $SERVER_IP $SERVER_PORT $SEND_BUFFER_SIZE >> $LOGS_DIR