#!/bin/bash

./pager 4 2 &
pager_pid=$!

sleep 1
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pager_pid

wait $pager_pid

