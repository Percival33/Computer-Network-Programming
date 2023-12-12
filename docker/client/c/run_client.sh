#!/bin/bash


# SERVER_IP is an env var
# echo $SERVER_IP > test.txt


SNDBUF_SIZE=$1


if [ -z $SNDBUF_SIZE ]; then
    while true; do
        ./client $SERVER_IP 8888
        sleep 5
        echo ""
    done
    exit 0
fi


while true; do
    ./client $SERVER_IP 8888 $SNDBUF_SIZE
    sleep 5
    echo ""
done