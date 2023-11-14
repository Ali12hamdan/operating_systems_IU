#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path_to_watch>"
    exit 1
fi

# Path to the directory to be monitored
MONITOR_DIR=$1

# Compile monitor.c and ex1.c
gcc -o monitor monitor.c -Wall
gcc -o ex1 ex1.c -Wall

# Run monitor.c in the background
./monitor "$MONITOR_DIR" &
MONITOR_PID=$!

# Wait for a bit to ensure monitor.c is initialized
sleep 2

# Run ex1.c
./ex1 "$MONITOR_DIR"

# Wait for a bit to ensure ex1.c has made its changes
sleep 2

# Send SIGINT to monitor.c to gracefully terminate it
kill -SIGINT "$MONITOR_PID"

# Wait for monitor.c to terminate
wait "$MONITOR_PID"
