#!/bin/bash


outDir="time"
dataDir="data"
data=( "graph50_6.txt" )
ks=( 11 )

rm -rf "$outDir"
mkdir "$outDir"

make all 1>/dev/null 2>/dev/null
for i in "${data[@]}"; do
	for k in "${ks[@]}"; do

		# name=$(echo $i | cut -d\  -f1)
		# k=$(echo $i | cut -d\  -f2)
		echo "Running job K: $k"
		echo -n "$k	" >> "$outDir/${i}"
		./pdp "$dataDir/$i" "$k" | sed -n 's/.*time: //p' >> "$outDir/${i}"
		#echo $k;
		#./pdp "$dataDir/$name" "$k" 
	done
done
