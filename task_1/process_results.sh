#!/bin/bash

cat bin_ins/longJob_*.sh.o* | grep '>>' | sed 's/ms//g' | sed 's/..\/samples\///g' | cut -f3,4,5,6,8,9,10 | sort -k5,5 -rk2,2 -k3n,3 

