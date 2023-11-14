#!/bin/bash

# ex22.sh

# 1. Check the inode, block size, total size, and permissions of ex1
echo "Checking inode, block size, total size, and permissions of ex1:"
stat ex1

# 2. Copy ex1 to ex2 and compare their inode numbers
echo "Copying ex1 to ex2 and comparing inode numbers:"
cp ex1 ex2
echo "Inode number for ex1:"
ls -i ex1
echo "Inode number for ex2:"
ls -i ex2

# 3. Check the number of links for ex2
echo "Checking the number of links for ex2:"
stat -c "%h" ex2

echo "Identifying files with 3 links in /etc:"
stat -c "%h - %n" /etc/* | grep ^3

