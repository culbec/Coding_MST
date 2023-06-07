#!/bin/bash

for i in $(seq 1 5); do
    ./main "./huffmannCoding/$i-in.txt" "./huffmannCoding/$i-out.txt"
done
