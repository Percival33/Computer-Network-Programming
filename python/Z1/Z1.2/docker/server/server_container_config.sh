#!/bin/bash

# tc qdisc add dev eth0 root netem delay 1000ms 500ms loss 50%

python ./server.py '127.0.0.1' 8888
# echo "hello World"