//C version scalaire 

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "rdtsc.h"

//
typedef struct {

  double x, y;
  
} vector;

//
int w, h;

//
int nbodies, timeSteps;

//
double *masses, GravConstant;

//
vector *positions, *velocities, *accelerations;

//
unsigned long long rdtsc(void)
{
  unsigned long long a, d;
  
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  
  return (d << 32) | a;
}

//
int randxy(int x, int y)
{
  return (rand() % (y - x + 1)) + x; 
}

//
double randreal()
{
  int s = (randxy(0, 1)) ? 1 : -1;
  int a = randxy(1, RAND_MAX), b = randxy(1, RAND_MAX);

  return s * ((double)a / (double)b); 
}

//
vector asm_add_vectors(vector a, vector b)
{

  vector c = {0,0};

  __asm__ volatile (
        "movsd (%[ax]), %%xmm0;\n"
        "movsd (%[bx]), %%xmm1;\n"

        "addsd %%xmm1, %%xmm0;\n"

        "movsd (%[ay]), %%xmm2;\n"
        "movsd (%[by]), %%xmm3;\n"

        "addsd %%xmm3, %%xmm2;\n"

        "movsd %%xmm0, (%[cx]);\n"
        "movsd %%xmm2, (%[cy]);\n"
        
        : //outputs
          
        : //inputs
          [ax] "r" (&a.x),
          [ay] "r" (&a.y),
          [bx] "r" (&b.x),
          [by] "r" (&b.y),
          [cx] "r" (&c.x),
          [cy] "r" (&c.y) 

          
        : //clobbers
          "cc", "memory", "xmm0", "xmm1", "xmm2", "xmm3"
        );
  
  return c;
}

vector add_vectors(vector a, vector b)
{
  vector c = { a.x + b.x, a.y + b.y };
  
  return c;
}

//
vector asm_scale_vector(double b, vector a)
{
  vector c = {0,0};

  __asm__ volatile (
        "movsd (%[ax]), %%xmm0;\n"
        "movsd (%[b]), %%xmm1;\n"

        "mulsd %%xmm0, %%xmm1;\n"
        "movsd %%xmm1, (%[cx]);\n"

        //
        "movsd (%[ay]), %%xmm2;\n"
        "movsd (%[b]), %%xmm1;\n"

        "mulsd %%xmm2, %%xmm1;\n"
        "movsd %%xmm1, (%[cy]);\n"
        
        : //outputs
          
        : //inputs
          [ax] "r" (&(a.x)),
          [ay] "r" (&(a.y)),
          [b] "r" (&b),
          [cx] "r" (&(c.x)),
          [cy] "r" (&(c.y)) 
          
        : //clobbers
          "cc", "memory", "xmm0", "xmm1", "xmm2"
        );
  
  return c;
}

vector scale_vector(double b, vector a)
{
  vector c = { b * a.x, b * a.y };
  
  return c;
}

//
vector asm_sub_vectors(vector a, vector b)
{
  vector c = {0,0};

  __asm__ volatile (
        "movsd (%[ax]), %%xmm0;\n"
        "movsd (%[bx]), %%xmm1;\n"

        "subsd %%xmm1, %%xmm0;\n"

        "movsd (%[ay]), %%xmm2;\n"
        "movsd (%[by]), %%xmm3;\n"

        "subsd %%xmm3, %%xmm2;\n"

        "movsd %%xmm0, (%[cx]);\n"
        "movsd %%xmm2, (%[cy]);\n"
        
        : //outputs
          
        : //inputs
          [ax] "r" (&(a.x)),
          [ay] "r" (&(a.y)),
          [bx] "r" (&(b.x)),
          [by] "r" (&(b.y)),
          [cx] "r" (&(c.x)),
          [cy] "r" (&(c.y)) 

          
        : //clobbers
          "cc", "memory", "xmm0", "xmm1", "xmm2", "xmm3"
        );
  
  return c;
}

vector sub_vectors(vector a, vector b)
{
  vector c = { a.x - b.x, a.y - b.y };
  
  return c;
}

//
double asm_mod(vector a)
{
  double b = 0.0;

   __asm__ volatile ( 
        "movsd (%[ax]), %%xmm0;\n"
        "mulsd %%xmm0, %%xmm0;\n"


        "movsd (%[ay]), %%xmm1;\n"
        "mulsd %%xmm1, %%xmm1;\n"

        "addsd %%xmm1, %%xmm0;\n"

        //
        "sqrtsd %%xmm0, %%xmm2;\n"
        
        "movsd %%xmm2, (%[b]);\n"
        
        : //outputs
          
        : //inputs
          [ax] "r" (&(a.x)),
          [ay] "r" (&(a.y)),
          [b] "r" (&b)
          
        : //clobbers
          "cc", "memory", "xmm0", "xmm1", "xmm2"
        );

  return b;
}

double mod(vector a)
{
  return sqrt(a.x * a.x + a.y * a.y);
}


//
void init_system()
{
  w = h = 800;
  nbodies = 600;
  GravConstant = 1;
  timeSteps = 1000;
  
  //
  masses        = malloc(nbodies * sizeof(double));
  positions     = malloc(nbodies * sizeof(vector));
  velocities    = malloc(nbodies * sizeof(vector));
  accelerations = malloc(nbodies * sizeof(vector));

  //
  for (int i = 0; i < nbodies; i++)
    {
      masses[i] = 5;
      
      positions[i].x = randxy(10, w);
      positions[i].y = randxy(10, h);
      
      velocities[i].x = randreal();
      velocities[i].y = randreal();
    }
}

//
void resolve_collisions()
{
  //
  for (int i = 0; i < nbodies - 1; i++)
    for (int j = i + 1; j < nbodies; j++)
      if (positions[i].x == positions[j].x &&
	  positions[i].y == positions[j].y)
	{
	  vector temp = velocities[i];
	  velocities[i] = velocities[j];
	  velocities[j] = temp;
	}
}

//
void compute_accelerations()
{ 
  for (int i = 0; i < nbodies; i++)
    {
      accelerations[i].x = 0;
      accelerations[i].y = 0;
      
      for(int j = 0; j < nbodies; j++)
	if(i != j)
	  accelerations[i] = asm_add_vectors(accelerations[i],
					 asm_scale_vector(GravConstant * masses[j] / (pow(asm_mod(asm_sub_vectors(positions[i], positions[j])), 3) + 1e7),
						      asm_sub_vectors(positions[j], positions[i])));
    }
}

//
void compute_velocities()
{  
  for (int i = 0; i < nbodies; i++)
    velocities[i] = asm_add_vectors(velocities[i], accelerations[i]);
}

//
void compute_positions()
{
  for (int i = 0; i < nbodies; i++)
    positions[i] = asm_add_vectors(positions[i], asm_add_vectors(velocities[i], asm_scale_vector(0.5, accelerations[i])));
}

//
void simulate()
{
  compute_accelerations();
  compute_positions();
  compute_velocities();
  resolve_collisions();
}

int test()
{ 
  vector test1 = {randreal(), randreal()}; 
  vector test2 = {randreal(), randreal()}; 

  double val = randreal(); 
  vector a = add_vectors(test1, test2); 
  vector b = asm_add_vectors(test1, test2); 

  if(a.x != b.x || a.y != b.y) 
    { printf("a.x = %lf, a.y = %lf, b.x = %lf, b.y = %lf\n",a.x,a.y,b.x,b.y);
      printf("addition \n");
return 1;} 

      a = scale_vector(val, test1); 
      b = asm_scale_vector(val, test1); 

      if(a.x != b.x || a.y != b.y) 
        { printf("a.x = %lf, a.y = %lf, b.x = %lf, b.y = %lf\n",a.x,a.y,b.x,b.y);
          printf("scalaire vecteur \n");
          return 1;} 

          a = sub_vectors(test1, test2); 
          b = asm_sub_vectors(test1, test2); 

          if(a.x != b.x || a.y != b.y) 
            { printf("a.x = %lf, a.y = %lf, b.x = %lf, b.y = %lf\n",a.x,a.y,b.x,b.y);
              printf("soustraction \n");
              return 1;} 

              double c = mod(test1); 
              double d = asm_mod(test1); 

              if(c != d) 
                { printf("c = %lf, d = %lf\n",c,d);
                  printf("racine \n");
                 return 1;} 

            return 0;
    }

//
int main(int argc, char **argv)
{
  //
  int i;
  unsigned char quit = 0;
  SDL_Event event;
  SDL_Window *window;
  SDL_Renderer *renderer;

  srand(time(NULL));
   
  if (test())
  {
    return 0;
  }
  
  //
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_OPENGL, &window, &renderer);
  
  //
  init_system();
  
  //Main loop
  for (int i = 0; !quit && i < timeSteps; i++)
    {	  
      //
      double before = (double)rdtsc();
      
      simulate();

      //
      double after = (double)rdtsc();
      
      //
      printf("%d %lf\n", i, (after - before));
      
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      
      for (int i = 0; i < nbodies; i++)
	{
	  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	  SDL_RenderDrawPoint(renderer, positions[i].x, positions[i].y);
	}
      
      SDL_RenderPresent(renderer);
      
      SDL_Delay(10);
      
      while (SDL_PollEvent(&event))
	if (event.type == SDL_QUIT)
	  quit = 1;
	else
	  if (event.type == SDL_KEYDOWN)
	    if (event.key.keysym.sym == SDLK_q)
	      quit = 1;
    }
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}