#!/bin/bash

rm -f output.txt 

while read line || [[ -n "$line" ]];
do
	echo $line
	echo $line > init.txt

	./genClau init.txt

	minisat out.cnf sat.txt 

	echo $line >> output.txt
	./genOut sat.txt $line[0] $line[1]
	echo >> output.txt

done < input.txt

rm -f aux.txt sat.txt out.cnf init.txt 
make clean
