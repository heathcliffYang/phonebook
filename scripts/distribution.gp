reset
set ylabel 'the number of layers'
set style fill solid
set title 'Distribution'
set term png enhanced font 'Verdana,10'
set output 'distribution.png'
set xtic 500
set xrange [0:3000]

plot [:][:]'eachDistribute.txt' using 1:2 with points title ''
