#!/bin/bash
# $1 archivo ejecutable
# $2 es el archivo de entrada (instancias)
#C=$(($C -1))

F="$3.csv"
C=$(cat $2 | wc -l)
i=$((1))

echo  "group, algorithm, domain, instance, cost, generated, time, states for sec " >> $F

while read line
do 
	G="$line"
	if [ \( $i -le $C \) ]; then
		timeout 5m ./$1 "$G" $F 
	fi
	i=$(($i +1))
done < $2
