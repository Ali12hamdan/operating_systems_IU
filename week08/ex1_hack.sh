#!/bin/bash

PID_FILE="/tmp/ex1.pid"
TARGET_PID=$(cat $PID_FILE)

MAPS_LINE=$(grep -F 'rw-s' /proc/$TARGET_PID/maps | head -n 1)
START_ADDR=$(echo $MAPS_LINE | awk '{print $1}' | cut -d'-' -f1)

PASSWORD_LINE=$(sudo xxd -s "0x$START_ADDR" /proc/$TARGET_PID/mem | grep "pass:" -a -m 1)

if [[ -n $PASSWORD_LINE ]]; then
    PASSWORD_MEMORY_ADDRESS=$(echo $PASSWORD_LINE | awk '{print $1}' | tr -d ':')
    PASSWORD_STR=$(echo $PASSWORD_LINE | awk '{for(i=10;i<=NF;i++) printf $i; print ""}')
    echo "Memory Address: 0x$PASSWORD_MEMORY_ADDRESS"
    echo "Password: $PASSWORD_STR"
else
    echo "Password not found."
fi

kill -9 $TARGET_PID
