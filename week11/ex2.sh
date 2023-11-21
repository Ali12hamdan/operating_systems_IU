# create the filesystem
dd bs=1024 count=128 if=/dev/zero of=disk0

# compile
gcc ex2.c -o ex2

# run

cat input.txt | ./ex2
