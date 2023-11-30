#!/bin/bash

# SERVER_IP is an env var
echo $SERVER_IP > test.txt

while true; do
    ./client.py $SERVER_IP 8888
    sleep 3
done