#!/bin/bash

# SERVER_IP is an env var
# echo $SERVER_IP > test.txt

while true; do
    ./client $SERVER_IP 8888
    sleep 3
done