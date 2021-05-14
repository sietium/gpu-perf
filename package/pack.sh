#!/bin/sh  
exe="GPU-Perf"
des="/home/sietium/TOOLS/gpu-perf/package"
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')  
cp $deplist $des
