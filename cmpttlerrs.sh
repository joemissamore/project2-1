#! /bin/bash

> diff.res;
make clean;
make;

cp P2Tests/*.ss .;

array=$(ls P2Tests/ | cut -d '.' -f1 | sort -u); # create list of files

for i in ${array[@]}
do
	./P2.out $i.ss;
	# Only comparing the number of errors
	# res=$(diff -qs <(tail -n 1 P2Tests/$i.lst) <(tail -n 1 $i.lst) | awk '{ print $NF }');
	# echo "$i.lst files: $res" >> diff.res;
	
	# Compare the entire output except the first line
	res=$(diff -qs <(tail -n +2 P2Tests/$i.lst) <(tail -n +2 $i.lst) | awk '{ print $NF }'); 
	echo "$i.lst files: $res" >> diff.res; 
done

rm -f *.ss
