#!/bin/bash
# $2 es el archivo de entrada (instancias)
# $1 archivo ejecutable
# $3 archivo de salida

P= "./$1"
I= "../instances/$2.txt"
O= "$3.csv"

echo "grupo, algorithm, domain, instance, cost, generated, time, gen_per_sec" >> $O

let _i=1
while read line
do 
	echo -e "$_i ---> $line\n"
	timeout -k 0.1m $P $line $2 >> $O
	if [$? == 124];then
		echo "X, dfid, $2,\"$line\",na, na, na, na"
	fi			
	let _i=$_i+1 
done < $I