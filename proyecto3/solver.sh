rm -f output.txt aux.txt out.cnf
while read line || [[ -n "$line" ]];
do
	echo $line
	echo $line > init.txt

	./genClau init.txt

	# minisat aqui
	minisat