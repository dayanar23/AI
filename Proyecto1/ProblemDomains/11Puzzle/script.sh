#!/bin/bash
# $1 archivo ejecutable
# $2 es el archivo de entrada (instancias)

F="$3.csv"

while read line
do 
	G=$line
	timeout 0.5m ./$1 "$G" $F
done < $2