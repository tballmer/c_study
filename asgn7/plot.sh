#!/usr/bin/env bash

echo -n "Plotting... "
gnuplot <<EOF
set terminal pdf
set key outside
set zeroaxis
set xlabel "Bloom Filter Size"
set ylabel "Lookups"
set output "bf_lookup.pdf"
set title "Lookups Decrease"
plot "bf_lookup.data" with linespoints title "Lookups"
EOF
echo "done."

