# create file with the requiered specification to back the loopback device
dd of=lofs.img bs=50M count=1 if=/dev/zero

# attach first free loop device to the created file
LOOP_DEVICE=$(sudo losetup -f lofs.img --show)

# create ext4 filesystem on the loop device
sudo mkfs.ext4 $LOOP_DEVICE

# create the mountpoint and mount the loopback device there
mkdir ./lofsdisk
sudo mount $LOOP_DEVICE lofsdisk
sudo chown $(ls -ld ex1.c | awk '{print $3":"$4}') lofsdisk

# create task files
cd lofsdisk
echo "Ali" >file1
echo "Hamdan" >file2

get_libs() {
    ldd $1 | awk '{$1=""; print$3}' | grep .so
}

# for every executable...
for name in bash ls cat echo; do
    # copy the file
    cp $(which $name) ./
    # copy the dynamic dependencies
    get_libs $(which $name) | while read line; do
        cp --parent $line ./;
    done
done


#I had a problem with this dependency and I fixed like this (I copied it specifically and it won't work without it)
# run
cp --parent /lib64/ld-linux-x86-64.so.2 ./

gcc ../ex1.c -o ex1

echo "WITH CHROOT:" >> ../ex1.txt

# running the program with chroot in the current working directory
sudo chroot ./ ./ex1 >> ../ex1.txt

echo "WITHOUT CHROOT" >> ../ex1.txt

# running the program without the chroot
./ex1 >> ../ex1.txt
