#!/bin/bash


outDir="time"
dataDir="data"
data=( "graph50_6.txt 11" )
threads=( 1 2 4 8 16 24 32 )

rm -rf "$outDir"
mkdir "$outDir"

for i in "${data[@]}"; do
	for t in "${threads[@]}"; do
		make all threads=$t 1>/dev/null 2>/dev/null

		name=$(echo $i | cut -d\  -f1)
		k=$(echo $i | cut -d\  -f2)
	
		echo "Running job T: $t"
		echo -n "$t	" >> "$outDir/${k}_${name}"
		./pdp "$dataDir/$name" "$k" | sed -n 's/.*time: //p' >> "$outDir/${k}_${name}"
		#./pdp "$dataDir/$name" "$k" 
	done
done
