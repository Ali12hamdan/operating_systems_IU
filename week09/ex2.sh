#!/bin/bash

gcc -o mmu mmu.c
gcc -o pager pager.c

./pager 10 12345 random &
PAGER_PID=$!
./mmu 10 "r0w1r2w3r4w5r6w7r8w9" $PAGER_PID random

wait $PAGER_PID
