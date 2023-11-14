#!/bin/bash
# gen.sh

# This script generates a file with random numbers
# Usage: ./gen.sh [number_of_random_numbers] [output_file]

for (( i=0; i<$1; i++ )); do
    echo $RANDOM >> $2
done
