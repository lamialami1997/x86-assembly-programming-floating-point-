set terminal png size 1900,1100
set grid  
set ylabel "Latency in cycles"  
set xlabel "Simulation iteration"  
plot "out0.dat" with lines title "C version AOS ", "out1.dat" with lines title "C version SCALAIRE", "out2.dat" with lines title "C version VECTEUR ", "out3.dat" with lines title "C version SOA"

