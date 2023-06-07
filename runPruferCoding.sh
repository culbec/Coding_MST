#!/bin/bash

for i in $(seq 1 9); do
    ./main "./pruferCoding/$i-in.txt" "./pruferCoding/$i-out.txt"
done
