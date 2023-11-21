# createa dfsd
dd of=lofs.img bs=50M count=1 if=/dev/zero


LOOP_DEVICE=$(sudo losetup -f lofs.img --show)
sudo mkfs.ext4 $LOOP_DEVICE
mkdir ./lofsdisk
sudo mount $LOOP_DEVICE lofsdisk
sudo chown $(ls -ld ex1.c | awk '{print $3":"$4}') lofsdisk
cd lofsdisk
echo "Ali" >file1
echo "Hamdan" >file2

get_libs() {
    ldd $1 | awk '{$1=""; print$3}' | grep .so
}

for name in bash ls cat echo; do
    cp $(which $name) ./
    get_libs $(which $name) | while read line; do
        cp --parent $line ./;
    done
done

cp --parent /lib64/ld-linux-x86-64.so.2 ./

gcc ../ex1.c -o ex1

echo "WITH CHROOT:" >> ../ex1.txt

sudo chroot ./ ./ex1 >> ../ex1.txt

echo "WITHOUT CHROOT" >> ../ex1.txt

./ex1 >> ../ex1.txt
