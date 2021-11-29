// C version SOA 

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "rdtsc.h"

//
int w, h;

//
int nbodies, timeSteps;

//
double *masses, GravConstant;

//
double *positionsx,*positionsy,*velocitiesx,*velocitiesy, *accelerationsx, *accelerationsy;

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
void init_system()
{
  w = h = 800;
  nbodies = 600;
  GravConstant = 1;
  timeSteps = 1000;
  
  // 
  masses       = malloc(nbodies * sizeof(double));
  // les points sont groupés dans deux  tableaux distincts 
  positionsx  = malloc(nbodies * sizeof(double));
  positionsy  = malloc(nbodies * sizeof(double));
  velocitiesx = malloc(nbodies * sizeof(double));
  velocitiesy = malloc(nbodies * sizeof(double));
  accelerationsx = malloc(nbodies * sizeof(double));
  accelerationsy = malloc(nbodies * sizeof(double));

  //
  for (int i = 0; i < nbodies; i++)
    {
      masses[i] = 5;
      
      positionsx[i] = randxy(10, w);
      positionsy[i] = randxy(10, h);
      
      velocitiesx[i] = randreal();
      velocitiesy[i]= randreal();
    }
}

//
void resolve_collisions()
{
  //
  for (int i = 0; i < nbodies - 1; i++)
    for (int j = i + 1; j < nbodies; j++)
      if (positionsx[i] == positionsx[j] &&
    positionsy[i] == positionsy[j])
  {
    double temp = velocitiesx[i];
    velocitiesx[i] = velocitiesx[j];
    velocitiesx[j] = temp;

    temp = velocitiesy[i];
    velocitiesy[i] = velocitiesy[j];
    velocitiesy[j] = temp;
  }
}

//
void compute_accelerations()
{ 
  for (int i = 0; i < nbodies; i++)
    {
      accelerationsx[i] = 0;
      accelerationsy[i] = 0;
      
      for(int j = 0; j < nbodies; j++)
      {
  if(i != j)
    {
      
      accelerationsx[i] = accelerationsx[i] + (GravConstant * masses[j] / 
        (pow(sqrt((positionsx[i]-positionsx[j])*(positionsx[i]-positionsx[j]) + (positionsy[i]-positionsy[j])*(positionsy[i]-positionsy[j])),3) + 1e7)) *
                  (positionsx[j] - positionsx[i]); 

      accelerationsy[i] = accelerationsy[i] + (GravConstant * masses[j] / 
        (pow(sqrt((positionsx[i]-positionsx[j])*(positionsx[i]-positionsx[j]) + (positionsy[i]-positionsy[j])*(positionsy[i]-positionsy[j])),3) + 1e7)) *
                  (positionsy[j] - positionsy[i]); 
    
    }}}

}

//
void compute_velocities()
{  
  for (int i = 0; i < nbodies; i++)
    { 
      //  add_vectors 
      velocitiesx[i] = (velocitiesx[i] + accelerationsx[i]);
      velocitiesy[i] = velocitiesy[i] + accelerationsy[i];

    }
}

//
void compute_positions()
{
  for (int i = 0; i < nbodies; i++)
  // add_vectors et scal_vector 
    {
      positionsx[i] = positionsx[i] + (velocitiesx[i] + 0.5 * accelerationsx[i]);
      positionsy[i] = positionsy[i] + (velocitiesy[i] + 0.5 * accelerationsy[i]);
    }
}

//
void simulate()
{
  compute_accelerations();
  compute_positions();
  compute_velocities();
  resolve_collisions();
}

//
int main(int argc, char **argv)
{
  //
  unsigned char quit = 0;
  SDL_Event event;
  SDL_Window *window;
  SDL_Renderer *renderer;

  srand(time(NULL));
  
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
    SDL_RenderDrawPoint(renderer, positionsx[i], positionsy[i]);
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
  
  free(masses);
  free(positionsx);
  free(positionsy);
  free(accelerationsx);
  free(accelerationsy);
  free(velocitiesx);
  free(velocitiesy);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}