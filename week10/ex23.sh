#!/bin/bash

# ex23.sh

# Creating a text file ex1.txt using gen.sh with arguments 10 and ex1.txt
./gen.sh 10 ex1.txt
# ex1.txt is now created with 10 random numbers.

# Creating symbolic links to ex1.txt
ln -s "$(realpath ex1.txt)" ex11.txt
ln -s "$(realpath ex1.txt)" ex12.txt
# Symbolic links ex11.txt and ex12.txt created pointing to ex1.txt

# Comparing the content of all files
echo "Comparing ex1.txt and ex11.txt:"
diff ex1.txt ex11.txt
echo "Comparing ex1.txt and ex12.txt:"
diff ex1.txt ex12.txt
# There should be no difference as ex11.txt and ex12.txt are symbolic links to ex1.txt

# Checking inode numbers
echo "Checking inode numbers:"
ls -i ex1.txt ex11.txt ex12.txt > output.txt
# Saved inode numbers to output.txt

# Checking disk usage
echo "Disk usage of ex1.txt:"
du ex1.txt

# Creating a hard link ex13.txt for ex1.txt and moving it to /tmp
ln ex1.txt ex13.txt
mv ex13.txt /tmp
# Hard link ex13.txt created and moved to /tmp

# Tracing all links to ex1.txt in the current and root path
echo "Tracing links in current path:"
find . -samefile ex1.txt
echo "Tracing links in root path:"
find / -samefile ex1.txt 2>/dev/null
# The 2>/dev/null is used to suppress permission denied errors when searching in /

# Checking the number of hard links for ex1.txt
echo "Number of hard links for ex1.txt:"
stat -c "%h" ex1.txt

# Removing all links from ex1.txt
find . -samefile ex1.txt -exec rm {} \;
echo "All links removed from ex1.txt"

# End of script
