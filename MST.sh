#!/bin/bash

for i in $(seq 1 10); do
    ./main "./MST/$i-in.txt" "./MST/$i-out.txt"
done
