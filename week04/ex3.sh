#!/bin/bash
gcc ex3.c -o ex3
./ex3 3 &
sleep 10
pstree -p $!
./ex3 5 &
sleep 20
pstree -p $!
rm ex3
