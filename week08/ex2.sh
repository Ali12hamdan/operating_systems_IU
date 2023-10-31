#!/bin/bash


gcc -o mmu mmu.c
gcc -o pager pager.c


./pager <number_of_pages> $$ &
pager_pid=$!


./mmu <number_of_pages> <reference_string> $pager_pid

kill $pager_pid
rm mmu pager
