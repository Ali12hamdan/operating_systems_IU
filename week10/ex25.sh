#!/bin/bash

# ex25.sh

# Make an empty file ex5.txt
echo "Creating an empty file ex5.txt."
touch ex5.txt

# Remove write permission for everybody
echo "Removing write permission for everybody."
chmod a-w ex5.txt
# The command sets permissions to --- (no write permission) for user, group, and others.

# Grant all permissions to owner and others (not group)
echo "Granting all permissions to owner and others (not group)."
chmod u=rwx,o=rwx ex5.txt
# Sets permissions to rwx for user and others, and --- for group.

# Make group permissions equal to user permissions
echo "Making group permissions equal to user permissions."
chmod g=u ex5.txt
# Sets group permissions equal to user permissions, resulting in rwx for both user and group.

# What does 660 mean for ex5.txt?
echo "Setting permissions to 660 for ex5.txt."
chmod 660 ex5.txt
# Permissions: rw- for user and group, --- for others.

# What does 775 mean for ex5.txt?
echo "Setting permissions to 775 for ex5.txt."
chmod 775 ex5.txt
# Permissions: rwx for user and group, r-x for others.

# What does 777 mean for ex5.txt?
echo "Setting permissions to 777 for ex5.txt."
chmod 777 ex5.txt
# Permissions: rwx for user, group, and others.

# Displaying final permissions
echo "Final permissions for ex5.txt:"
ls -l ex5.txt
# Expected: -rwxrwxrwx showing full permissions for user, group, and others.

# End of script
