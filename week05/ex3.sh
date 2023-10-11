gcc -o ex3 ex3.c -pthread

n=10000000
for m in 1 2 4 10 100; do
    echo "Testing with m = $m..."
    time ./ex3 $n $m
done

rm ex3
