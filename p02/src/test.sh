#!/bin/bash

num1=$1
num2=$2
echo $((num1+num2)) $((num1-num2))>answer.txt 
make
echo $num1 $num2 | main.out > output.txt
diff -c answer.txt output.txt > result.txt
make clean
rm output.txt answer.txt
