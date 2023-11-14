#!/bin/bash

# ex25.sh

echo "Creating an empty file ex5.txt."
touch ex5.txt

echo "Removing write permission for everybody."
chmod a-w ex5.txt

echo "Granting all permissions to owner and others (not group)."
chmod u=rwx,o=rwx ex5.txt

echo "Making group permissions equal to user permissions."
chmod g=u ex5.txt

echo "Setting permissions to 660 for ex5.txt."
chmod 660 ex5.txt

echo "Setting permissions to 775 for ex5.txt."
chmod 775 ex5.txt

echo "Setting permissions to 777 for ex5.txt."
chmod 777 ex5.txt

echo "Final permissions for ex5.txt:"
ls -l ex5.txt

