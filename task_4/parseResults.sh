#!/bin/bash

rm -rf times
mkdir times
for procs in 2 4 8 16 24 32
do
	for threads in 1 2 4
	do
		found=false
		total=0
		for f in parallel_job.sh.o*
		do
			p=$(grep -E "^NODES: ${procs}$" $f)
			t=$(grep -E "^THREADS: ${threads}$" $f)
			if ! [[ -z "$p" ]] && ! [[ -z "$t" ]]; then
				found=true
				total=$(sed -n "s/Total elapsed time: \(.*\)s/\1/p" $f | sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[m|K]//g")
				break
			fi
			# if [[ $(grep -E "^NODES: ${procs}$" $f) == 0 ]]; then
				# echo "res"
			# fi
		done

		if [[ ! $found ]]; then
			echo "P: ${procs}, T: ${threads} not found!"
		fi

		echo "${procs}	${total}" >> "times/threads_${threads}.txt"
	done
done


