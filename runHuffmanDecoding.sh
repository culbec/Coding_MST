#!/bin/bash

for i in $(seq 1 5); do
    ./main "./huffmannDecoding/$i-in.txt" "./huffmannDecoding/$i-out.txt"
done
