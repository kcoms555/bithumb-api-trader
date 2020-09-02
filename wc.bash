#! /bin/bash
num=0
num2=0
filelist=($(find . -name '*.cpp' -o -name '*.h' ))
for i in ${filelist[@]}
do
	echo $i
	data=($(wc -l $i))
	num=$(($num + ${data[0]}))
	num2=$(expr $num2 + ${data[0]})
done
echo $num
echo $num2
