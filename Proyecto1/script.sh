#!/bin/bash
# $1 es el archivo de entrada (instancias)
# $2 archivo ejecutable
# $3 archivo de salida

while read line
do
	com = ./$2 $line $3
  	timeout --kill 2.0m $com
done < $1