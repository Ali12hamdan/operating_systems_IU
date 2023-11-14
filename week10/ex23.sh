#!/bin/bash

# ex23.sh

./gen.sh 10 ex1.txt
# ex1.txt is now created with 10 random numbers.

ln -s "$(realpath ex1.txt)" ex11.txt
ln -s "$(realpath ex1.txt)" ex12.txt

# Comparing the content of all files
echo "Comparing ex1.txt and ex11.txt:"
diff ex1.txt ex11.txt
echo "Comparing ex1.txt and ex12.txt:"
diff ex1.txt ex12.txt

# Checking inode numbers
echo "Checking inode numbers:"
ls -i ex1.txt ex11.txt ex12.txt > output.txt

echo "Disk usage of ex1.txt:"
du ex1.txt

ln ex1.txt ex13.txt
mv ex13.txt /tmp


echo "Tracing links in current path:"
find . -samefile ex1.txt
echo "Tracing links in root path:"
find / -samefile ex1.txt 2>/dev/null

echo "Number of hard links for ex1.txt:"
stat -c "%h" ex1.txt

find . -samefile ex1.txt -exec rm {} \;
echo "All links removed from ex1.txt"

