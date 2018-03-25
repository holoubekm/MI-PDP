#!/bin/bash

INFINIBAND="false"
IDIR="time/infiniband/"
EDIR="time/ethernet/"
TASK="./pdp ./data/graph40_5.txt 19"

if [[ ${INFINIBAND} = "true" ]]; then
	rm -rf "${IDIR}"
else
	rm -rf "${EDIR}"
fi

mkdir -p "${IDIR}"
mkdir -p "${EDIR}"

make compile
for PROCS in 2 4 8 16 24 32
do
	for THREADS in 1 2 4
	do
		echo "Starting P: ${PROCS} x T: ${THREADS}"
		if [[ ${INFINIBAND} = "true" ]]
		then
			mpirun --mca btl tcp,self --hostfile starHosts -np ${PROCS} --map-by ppr:1:node:pe=${THREADS} ${TASK} ${THREADS} >> "${IDIR}/output_${THREADS}_${PROCS}.txt"
		else
			mpirun --hostfile starHosts -np ${PROCS} --map-by ppr:1:node:pe=${THREADS} ${TASK} ${THREADS} >> "${EDIR}/output_${THREADS}_${PROCS}.txt"
		fi
		echo "Job finished"
	done
done
