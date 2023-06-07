#!/bin/bash

for i in $(seq 1 9); do
    ./main "./pruferDecoding/$i-in.txt" "./pruferDecoding/$i-out.txt"
done
