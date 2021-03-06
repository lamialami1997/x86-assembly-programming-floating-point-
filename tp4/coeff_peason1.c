#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define alignement 64  // alignement => 64 bits 

    uint64_t rdtsc()
{
	int rdtsc;
	__asm__ volatile("rdtsc;\n" : "=A" (rdtsc));
	return rdtsc;
}
   
	double correlationCoefficient(double* restrict X, double* restrict Y, int n)
	{
		double  sum_X = 0, sum_Y = 0, sum_XY = 0;
		double squareSum_X = 0, squareSum_Y = 0;
		
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
		for (int n =5 ; n<100 ; n++)
		{
		// allouer et remplire des deux tableau aléatoirement 
        
		double* restrict X= aligned_alloc(alignement, n*sizeof(double));

            for(int i = 0 ; i < n; i++)

            {
                
                 X[i] = rand() %100 ;
                
            }
            
        double* restrict Y= aligned_alloc(alignement, n*sizeof(double));

            for(int i = 0 ; i < n; i++)

            {
                
                 Y[i] = rand() %100 ;
                
            }
		
		
		// calculer le temps d'execution 
		double corr ;
		uint64_t sum_rdtsc = 0;
		for (int i=0; i<100 ; i++)
		{
			 uint64_t before = rdtsc();
			 
		
	    	 corr = correlationCoefficient(X, Y, n);
           
			 uint64_t after = rdtsc();
			 sum_rdtsc+=after-before;

		}
       
		printf("%lf %d %lu\n",corr ,n ,sum_rdtsc/100);
	free(X);
	free(Y);

		}
		

        
		
	}





