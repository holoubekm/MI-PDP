set terminal eps enhanced 

set xrange [1:32]
set xlabel ""
set ylabel "Spedup"
set termoption dash

set output "mpi_speedup_19_graph40_6.eps"
plot "mpi_speedup_19_data40_6.txt" using 1:2 title "Measured speedup - 1 proc" with lines axes x1y1 lc rgb "red" lw 2, \
	"mpi_speedup_19_data40_6.txt" using 1:3 title "Expected speedup - 1 proc" with lines axes x1y1 lc rgb "red" lw 1, \
	"mpi_speedup_19_data40_6.txt" using 1:4 title "Measured speedup - 2 procs" with lines axes x1y1 lc rgb "blue" lw 2, \
	"mpi_speedup_19_data40_6.txt" using 1:5 title "Expected speedup - 2 procs" with lines axes x1y1 lc rgb "blue" lw 1, \
	"mpi_speedup_19_data40_6.txt" using 1:6 title "Measured speedup - 4 procs" with lines axes x1y1 lc rgb "green" lw 2, \
	"mpi_speedup_19_data40_6.txt" using 1:7 title "Expected speedup - 4 procs" with lines axes x1y1 lc rgb "green" lw 1

set output "mpi_speedup_19_graph40_5.eps"
plot "mpi_speedup_19_data40_5.txt" using 1:2 title "Measured speedup - 1 proc" with lines axes x1y1 lc rgb "red" lw 2, \
	"mpi_speedup_19_data40_5.txt" using 1:3 title "Expected speedup - 1 proc" with lines axes x1y1 lc rgb "red" lw 1, \
	"mpi_speedup_19_data40_5.txt" using 1:4 title "Measured speedup - 2 procs" with lines axes x1y1 lc rgb "blue" lw 2, \
	"mpi_speedup_19_data40_5.txt" using 1:5 title "Expected speedup - 2 procs" with lines axes x1y1 lc rgb "blue" lw 1, \
	"mpi_speedup_19_data40_5.txt" using 1:6 title "Measured speedup - 4 procs" with lines axes x1y1 lc rgb "green" lw 2, \
	"mpi_speedup_19_data40_5.txt" using 1:7 title "Expected speedup - 4 procs" with lines axes x1y1 lc rgb "green" lw 1

set output "mpi_speedup_11_graph50_6.eps"
plot "mpi_speedup_11_data50_6.txt" using 1:2 title "Measured speedup - 1 proc" with lines axes x1y1 lc rgb "red" lw 2, \
	"mpi_speedup_11_data50_6.txt" using 1:3 title "Expected speedup - 1 proc" with lines axes x1y1 lc rgb "red" lw 1, \
	"mpi_speedup_11_data50_6.txt" using 1:4 title "Measured speedup - 2 procs" with lines axes x1y1 lc rgb "blue" lw 2, \
	"mpi_speedup_11_data50_6.txt" using 1:5 title "Expected speedup - 2 procs" with lines axes x1y1 lc rgb "blue" lw 1, \
	"mpi_speedup_11_data50_6.txt" using 1:6 title "Measured speedup - 4 procs" with lines axes x1y1 lc rgb "green" lw 2, \
	"mpi_speedup_11_data50_6.txt" using 1:7 title "Expected speedup - 4 procs" with lines axes x1y1 lc rgb "green" lw 1

# set output "mp_speedup_19_graph40_6.eps"
# plot "mp_speedup_19_data40_6.txt" using 1:1 title "Expected speedup" with lines axes x1y1 lc rgb "red" lw 1, \
# 	"mp_speedup_19_data40_6.txt" using 1:2 title "Tasks par. speed up" with lines axes x1y1 lc rgb "green" lw 2, \
# 	"mp_speedup_19_data40_6.txt" using 1:3 title "Data par. speed up" with lines axes x1y1 lc rgb "blue" lw 2

# set output "mp_speedup_19_graph40_5.eps"
# plot "mp_speedup_19_data40_5.txt" using 1:1 title "Expected speedup" with lines axes x1y1 lc rgb "red" lw 1, \
# 	"mp_speedup_19_data40_5.txt" using 1:2 title "Tasks par. speed up" with lines axes x1y1 lc rgb "green" lw 2, \
# 	"mp_speedup_19_data40_5.txt" using 1:3 title "Data par. speed up" with lines axes x1y1 lc rgb "blue" lw 2

# set output "mp_speedup_11_graph50_6.eps"
# plot "mp_speedup_11_data50_6.txt" using 1:1 title "Expected speedup" with lines axes x1y1 lc rgb "red" lw 1, \
# 	"mp_speedup_11_data50_6.txt" using 1:2 title "Tasks par. speed up" with lines axes x1y1 lc rgb "green" lw 2, \
# 	"mp_speedup_11_data50_6.txt" using 1:3 title "Data par. speed up" with lines axes x1y1 lc rgb "blue" lw 2
