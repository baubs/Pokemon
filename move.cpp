// Tyler Sammons                                                                                          
// Move a dot across screen using SDL                                                                     

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

int main( int argc, char* args[])
{
  //screen surface                                                                                        
  SDL_Surface* screen = NULL;
  SDL_Surface* dot = SDL_LoadBMP("dot.bmp");

  //screen dimensions                                                                                     
  const int sWidth = 640;
  const int sHeight = 480;
  //dot dimensions                                                                                        
  const int dWidth = 50;
  const int dHeight = 50;

  //position of dot                                                                                       
  int x = 600;
  int y = 400;

  //speed of dot                                                                                          
  int xVel = -5;
  int yVel = -5;

  //SDL setup                                                                                             
  SDL_Init( SDL_INIT_EVERYTHING );

  screen = SDL_SetVideoMode( sWidth, sHeight, 32, SDL_SWSURFACE );
  int imgFlags = IMG_INIT_PNG;

  if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
      printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }


  SDL_Rect src, dst;

  for(int i = 0; i <= 100; i++)
    {
      SDL_FillRect(screen, &dst, SDL_MapRGB(screen->format, 0, 0, 0));
      //the dot(rect)                                                                                     

      dst.x = x;
      dst.y = y;
      dst.w = dWidth;
      dst.h = dHeight;
      src.x = 0;
      src.y = 0;
      src.w = dWidth;
      src.h = dHeight;

      SDL_BlitSurface(dot , &src, screen, &dst);
      x += xVel;
      y += yVel;

      SDL_Delay( 25 );
      SDL_Flip( screen );
    }

  SDL_Quit();

  return 0;
}
