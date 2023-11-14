#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path_to_watch>"
    exit 1
fi


MONITOR_DIR=$1

gcc -o monitor monitor.c -Wall
gcc -o ex1 ex1.c -Wall

./monitor "$MONITOR_DIR" &
MONITOR_PID=$!

sleep 2

./ex1 "$MONITOR_DIR"

sleep 2

kill -SIGINT "$MONITOR_PID"

wait "$MONITOR_PID"
