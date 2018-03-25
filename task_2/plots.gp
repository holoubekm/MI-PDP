set terminal png size 640,480 enhanced 

set xlabel "Number of threads"
set ylabel "Total wall clock time (s)"
set output "graphs/10_graph50_6.png"
plot "output/10_graph50_6.txt" using 1:2 title "Wall clock x threads count" with lines axes x1y1

set output "graphs/10_graph50_6.png"
plot "output/10_graph50_6.txt" using 1:2 title "Wall clock x threads count" with lines axes x1y1

set output "graphs/11_graph50_6.png"
plot "output/11_graph50_6.txt" using 1:2 title "Wall clock x threads count" with lines axes x1y1

set output "graphs/10_graph50_6.png"
plot "output/10_graph50_6.txt" using 1:2 title "Wall clock x threads count" with lines axes x1y1

set output "graphs/12_graph50_6.png"
plot "output/12_graph50_6.txt" using 1:2 title "Wall clock x threads count" with lines axes x1y1

set output "graphs/15_graph30_5.png"
plot "output/15_graph30_5.txt" using 1:2 title "Wall clock x threads count" with lines axes x1y1

set output "graphs/15_graph40_5.png"
plot "output/15_graph40_5.txt" using 1:2 title "Wall clock x threads count" with lines axes x1y1

set output "graphs/20_graph40_5.png"
plot "output/20_graph40_5.txt" using 1:2 title "Wall clock x threads count" with lines axes x1y1
