#!/bin/bash

tc qdisc add dev eth0 root netem delay 1000ms 500ms loss 50%
python3 ./server.py 8888