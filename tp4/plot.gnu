set terminal png size 1900,1100
set output "coeff_peason.png"
set grid 
set ylabel "temps d'execution" 
set xlabel " size_ofXY"
plot "out.dat" using 2:3  with lines title "coeff_peason" 