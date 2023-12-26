#!/bin/sh

SEND_BUFFER_SIZE=$1
if [ -z "$SEND_BUFFER_SIZE" ]; then
    echo "INVALID ARG COUNT"
    exit 1
fi

CLIENT_PROGRAM='./build/output/client'
SERVER_IP='127.0.0.1'
SERVER_PORT=8888
LOGS_DIR="./experiment_2_logs/sndbuf_${SEND_BUFFER_SIZE}kB.txt"

# Clear the file
cat /dev/null > $LOGS_DIR

# Fill the file with new data
$CLIENT_PROGRAM $SERVER_IP $SERVER_PORT $SEND_BUFFER_SIZE >> $LOGS_DIR
