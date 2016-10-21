#!/bin/bash
# $1 archivo ejecutable
# $2 es el archivo de entrada (instancias)
#

F="$3.csv"
C=$(cat $2 | wc -l)
C=$(($C +1))
i=$((1))

echo "group, algorithm, heuristic, domain, instance, cost, h0, generated, time, states for sec " >> $F

while read line
do 
	G="$line"
	if [ \( $i -le $C \) ]; then
		timeout -s 2 10m ./$1 "$G" $F $3
	fi
	i=$(($i +1))
done < $2
