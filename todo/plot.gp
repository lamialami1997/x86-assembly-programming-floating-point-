set terminal png size 1900,1050 
set grid  
set ylabel "Latency in cycles"  
set xlabel "Simulation iteration"  
plot "out0.dat" with lines title "C version sans structure ", "out1.dat" with lines title "C version scalaire", "out2.dat" with lines title "C version vecteur", "out3.dat" with lines title "C version AOS"
