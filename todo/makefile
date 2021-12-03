all:  nbody_soa nbody_scal nbody_vect nbody_aos


nbody_soa: nbody_soa.c
	gcc -g -Ofast -funroll-loops -finline-functions -ftree-vectorize $< -o $@ -lm -lSDL2 
nbody_scal: nbody_scal.c
	gcc -g -Ofast -funroll-loops -finline-functions -ftree-vectorize $< -o $@ -lm -lSDL2 
nbody_vect: nbody_vect.c
	gcc -g -Ofast -funroll-loops -finline-functions -ftree-vectorize $< -o $@ -lm -lSDL2
nbody_aos: nbody_aos.c
	gcc -g -Ofast -funroll-loops -finline-functions -ftree-vectorize $< -o $@ -lm -lSDL2 

run:  nbody_soa nbody_scal nbody_vect nbody_aos
	
	sudo taskset -c 1 ./nbody_soa > out0.dat
	sudo taskset -c 1 ./nbody_scal > out1.dat
	sudo taskset -c 1 ./nbody_vect > out2.dat
	sudo taskset -c 1 ./nbody_aos > out3.dat
	gnuplot ./plot.gp > out.png
clean:
	rm -Rf *~ nbody_soa nbody_scal nbody_vect nbody_aos *.dat out.png
