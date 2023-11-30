#!/bin/bash

# SERVER_IP is an env var
echo $SERVER_IP > test.txt
./client $SERVER_IP 8888
