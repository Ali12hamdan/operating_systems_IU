#!/bin/bash

# ex24.sh

if [ -d "./tmp" ]; then
    echo "Deleting existing ./tmp folder."
    rm -rf ./tmp
fi

# Creating a symbolic link tmp1 for the folder ./tmp
echo "Creating a symbolic link named tmp1 for ./tmp."
ln -s "$(pwd)/tmp" tmp1

echo "Running ls -li:"
ls -li

# Creating the folder ./tmp
echo "Creating the folder ./tmp."
mkdir ./tmp

# Run ls -li again
echo "Running ls -li after creating ./tmp:"
ls -li

echo "Creating a file ex1.txt in ./tmp using gen.sh."
./gen.sh 10 ./tmp/ex1.txt

# Check the folder ./tmp1
echo "Checking the contents of ./tmp1:"
ls -l ./tmp1

echo "Creating a symbolic link named tmp2 within tmp1, pointing to ./tmp."
ln -s "$(realpath ./tmp)" ./tmp1/tmp2

echo "Creating another file ex1.txt in ./tmp1/tmp2 using gen.sh."
./gen.sh 10 ./tmp1/tmp2/ex1.txt

# Check the content of the sub-folders
echo "Checking the contents of ./tmp1/tmp2:"
ls -l ./tmp1/tmp2

echo "Accessing nested symbolic links:"
ls -l ./tmp1/tmp2/tmp2/tmp2/...

# Delete the folder ./tmp
echo "Deleting the folder ./tmp."
rm -rf ./tmp

# Check the symbolic links again
echo "Checking symbolic links after deleting ./tmp:"
ls -l tmp1 tmp1/tmp2

echo "Deleting all created symbolic links."
rm -f tmp1 tmp1/tmp2

echo "All operations completed."

