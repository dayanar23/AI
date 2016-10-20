#!/bin/bash
# $1 archivo ejecutable
# $2 es el archivo de entrada (instancias)

F="$3.csv"
C="cat $2 | wc -l" 
let i=1
while read line
do 
	G="$line"
	if [\( $i -l $C \)]; then
		timeout 5m ./$1 "$G" $F
	fi
done < $2
