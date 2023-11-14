#!/bin/bash

# ex24.sh

# Deleting the ./tmp folder if it exists
if [ -d "./tmp" ]; then
    echo "Deleting existing ./tmp folder."
    rm -rf ./tmp
fi

# Creating a symbolic link tmp1 for the folder ./tmp
echo "Creating a symbolic link named tmp1 for ./tmp."
ln -s "$(pwd)/tmp" tmp1

# Run ls -li
echo "Running ls -li:"
ls -li
# Expected: tmp1 will be listed as a symbolic link pointing to a non-existent ./tmp

# Creating the folder ./tmp
echo "Creating the folder ./tmp."
mkdir ./tmp

# Run ls -li again
echo "Running ls -li after creating ./tmp:"
ls -li
# Expected: ./tmp will now exist as a directory, and tmp1 still points to ./tmp

# Creating a file ex1.txt using gen.sh and adding it to ./tmp
echo "Creating a file ex1.txt in ./tmp using gen.sh."
./gen.sh 10 ./tmp/ex1.txt

# Check the folder ./tmp1
echo "Checking the contents of ./tmp1:"
ls -l ./tmp1
# Expected: ./tmp1 should show the same contents as ./tmp, including ex1.txt

# Creating another symbolic link tmp1/tmp2 to the folder ./tmp using only absolute paths
echo "Creating a symbolic link named tmp2 within tmp1, pointing to ./tmp."
ln -s "$(realpath ./tmp)" ./tmp1/tmp2

# Create another file ex1.txt using gen.sh and add it to the folder ./tmp1/tmp2
echo "Creating another file ex1.txt in ./tmp1/tmp2 using gen.sh."
./gen.sh 10 ./tmp1/tmp2/ex1.txt

# Check the content of the sub-folders
echo "Checking the contents of ./tmp1/tmp2:"
ls -l ./tmp1/tmp2
# Expected: The folder will show contents similar to ./tmp, including both instances of ex1.txt

# Trying to access the nested symbolic links
echo "Accessing nested symbolic links:"
ls -l ./tmp1/tmp2/tmp2/tmp2/...
# Expected: This will result in an infinite loop of symbolic links

# Delete the folder ./tmp
echo "Deleting the folder ./tmp."
rm -rf ./tmp

# Check the symbolic links again
echo "Checking symbolic links after deleting ./tmp:"
ls -l tmp1 tmp1/tmp2
# Expected: The symbolic links will now point to a non-existent location

# Delete all other symbolic links
echo "Deleting all created symbolic links."
rm -f tmp1 tmp1/tmp2

echo "All operations completed."

# End of script
