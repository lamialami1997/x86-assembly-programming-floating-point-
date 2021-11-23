all:  nbody0 nbody1a nbody1b nbodydo


nbody0: nbody0.c
	gcc -g -Ofast -funroll-loops -finline-functions -ftree-vectorize $< -o $@ -lm -lSDL2 
nbody1a: nbody1a.c
	gcc -g -Ofast -funroll-loops -finline-functions -ftree-vectorize $< -o $@ -lm -lSDL2 
nbody1b: nbody1b.c
	gcc -g -Ofast -funroll-loops -finline-functions -ftree-vectorize $< -o $@ -lm -lSDL2
nbodydo: nbodydo.c
	gcc -g -Ofast -funroll-loops -finline-functions -ftree-vectorize $< -o $@ -lm -lSDL2 

run:  nbody0 nbody1a nbody1b nbodydo
	
	sudo taskset -c 1 ./nbody0 > out0.dat
	sudo taskset -c 1 ./nbody1a > out1.dat
	sudo taskset -c 1 ./nbody1b > out2.dat
	sudo taskset -c 1 ./nbodydo > out3.dat
	gnuplot ./plot.gp > out.png
clean:
	rm -Rf *~ nbody0 nbody1a nbody1b nbodydo *.dat out.png