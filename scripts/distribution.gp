reset
set ylabel 'the number of layers'
set style fill solid
set title 'hash function distribution'
set term png enhanced font 'Verdana,10'
set output 'distribution.png'
set xrange[0:30000]
set xtics 2500

plot [:][:]'eachDistribute.txt' using 1:2 with points title 'BKDR'
