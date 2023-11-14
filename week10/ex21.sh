#!/bin/bash

# ex21.sh

# 1. Check file types in /dev and /etc
# ls -l /dev
# ls -l /etc

# 2. Count the number of directories in /etc
echo "Counting the number of directories in /etc:"
dir_count=$(ls -l /etc | grep ^d | wc -l)
echo "Number of directories in /etc: $dir_count"

# 3. Write a Hello World program in C
echo "Writing Hello World program in C"
echo '#include <stdio.h>' > ex1.c
echo 'int main() {' >> ex1.c
echo '    printf("Hello, world!\\n");' >> ex1.c
echo '    return 0;' >> ex1.c
echo '}' >> ex1.c

# Check file type of ex1.c
echo "Checking file type of ex1.c"
file ex1.c

# Compile ex1.c
echo "Compiling ex1.c"
gcc ex1.c -o ex1

# Check file type of compiled program
echo "Checking file type of compiled program ex1"
file ex1

# Modify ex1.c to print "Привет, мир!"
echo "Modifying ex1.c to print Привет, мир!"
sed -i 's/Hello, world!/Привет, мир!/' ex1.c

# Re-compile ex1.c
echo "Re-compiling ex1.c"
gcc ex1.c -o ex1

# Check file type of modified program
echo "Checking file type of modified program ex1"
file ex1

# 4. Retrieve file metadata and inode numbers
echo "Retrieving file metadata and inode numbers for ex1.c"
stat ex1.c
ls -i ex1.c
stat -c "%i" ex1.c
#stat -c "%i" ex1.c
