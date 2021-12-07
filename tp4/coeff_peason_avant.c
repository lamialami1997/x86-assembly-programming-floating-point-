#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

    uint64_t rdtsc()
{
	int rdtsc;
	__asm__ volatile("rdtsc;\n" : "=A" (rdtsc));
	return rdtsc;
}
   int n = 5 ;
	double correlationCoefficient(uint8_t* X, uint8_t* Y, int n)
	{
		int sum_X = 0, sum_Y = 0, sum_XY = 0;
		int squareSum_X = 0, squareSum_Y = 0;
		
		for (int i = 0; i < n; i++)
		{
			// somme des elements du tableau X 
			sum_X = sum_X + X[i];
		
			// somme des elements du tableau Y
			sum_Y = sum_Y + Y[i];
		
			// somme du produit XY  X[i] * Y[i].
			sum_XY = sum_XY + X[i] * Y[i];
		
			// somme du carré des éléments du tableau.
			squareSum_X = squareSum_X + X[i] * X[i];
			squareSum_Y = squareSum_Y + Y[i] * Y[i];
		}
		
		// calcul du coeffiecient de jeason 
		double corr = (n * sum_XY - sum_X * sum_Y)/
					sqrt((n * squareSum_X -
					sum_X * sum_X) * (n * squareSum_Y -
					sum_Y * sum_Y));
		
		return corr;
	}
		

	int main(int argc, char *argv[]) 
	{
		srand(time(NULL));
        
		// allouer et remplire des deux tableau aléatoirement 
        uint8_t *X= malloc(n*sizeof(uint8_t));

            for(int i = 0 ; i < n; i++)

            {
                
                 X[i] = rand() %100 ;
                
            }
            
        uint8_t *Y= malloc(n*sizeof(uint8_t));

            for(int i = 0 ; i < n; i++)

            {
                
                 Y[i] = rand() %100 ;
                
            }
		
		
		// calculer le temps d'execution 
		
        uint64_t before = rdtsc();
		
	     double corr = correlationCoefficient(X, Y, n);
           
		uint64_t after = rdtsc();

		printf("le temps d'execution est %lu\n",after - before);
	free(X);
	free(Y);
	}