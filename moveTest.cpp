// Brandon Aubry, Morris LaGrand, Tyler Sammons
// Pokemon test file

#include <iostream>
#include <stdlib.h>
#include "Pokemon.h"
#include "move.h"
#include <vector>
#include "ash.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>
using namespace std;


//Screen attributes
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;
const int SCREEN_BPP = 32;
const int frontWidth = 160;
const int frontHeight = 160;
const int backWidth = 96;
const int backHeight = 96;

//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *choices = NULL;
SDL_Surface *beige = NULL;
SDL_Surface *message1 = NULL;
SDL_Surface *message2 = NULL;
SDL_Surface *message3 = NULL;
SDL_Surface *message4 = NULL;
SDL_Surface *message5 = NULL;
SDL_Surface *message6 = NULL;
SDL_Surface *message7 = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *frontSprites = NULL;
SDL_Surface *backSprites = NULL;

// Declare clips
SDL_Rect clips;
SDL_Rect clips2;

//The event structure
SDL_Event event;

//The font that's going to be used
TTF_Font *font = NULL;
TTF_Font *font2 = NULL;

//The color of the font
SDL_Color textColor = { 0, 0, 0 };

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image
    loadedImage = IMG_Load( filename.c_str() );
    
    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //Free the old surface
        SDL_FreeSurface( loadedImage );
        
        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0xFF, 0xFF, 0xFF ) );
        }
    }
    
    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;
    
    //Get offsets
    offset.x = x;
    offset.y = y;
    
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
    
    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    
    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }
    
    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }
    
    //Set the window caption
    SDL_WM_SetCaption( "TTF Test", NULL );
    
    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the surfaces
    background = load_image( "battle_screen_resized.png" );
    frontSprites = load_image( "pokemon_front_sprites.png" );
    backSprites = load_image( "pokemon_back_sprites.png" );
    choices = load_image( "choices.png" );
    beige = load_image( "beige.png" );

    //Open the font
    font = TTF_OpenFont( "Chunkfive.ttf", 28 );
    font2 = TTF_OpenFont( "Chunkfive.ttf", 15);
    
    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }
    
    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }
    
    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( message1 );
    SDL_FreeSurface( message2 );

    //Close the font that was used
    TTF_CloseFont( font );

    bool quit = false;
    
    //Quit SDL
    SDL_Quit();
}

// Function to clip proper front pokemon sprite
void frontClips(int n) {
  
  int x;
  int y;

  // set x and y vals
  if (n%16 == 0)
    x = 15;
  else
    x = ((n%16)-1);

  if (n%16 == 0)
    y = (n/16) - 1;
  else
    y = (n/16);

  // clip sprites
  clips.x = x*frontWidth;
  clips.y = y*frontHeight;
  clips.w = frontWidth;
  clips.h = frontWidth;
  
}

void backClips(int n) {
  
  int x;
  int y;

  // set x and y vals
  if (n%20 == 0)
    x = 19;
  else
    x = ((n%20)-1);

  if (n%20 == 0)
    y = (n/20) - 1;
  else y = (n/20);

  // clip sprites
  clips2.x = x*backWidth;
  clips2.y = y*backHeight;
  clips2.w = backWidth;
  clips2.h = backHeight;
  
}




int main(int argc, char* args[]) {

  //seed time
  srand(time(NULL));

  // Walk around map
  //go();

    // SDL Stuff
    bool quit = false;
   
    //Initialize
    if( init() == false )
    {
        return 1;
    }
    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    // Create BattleScreen
    apply_surface( 0, 0, choices, screen );

    //Update the screen
    if( SDL_Flip( screen ) == -1 )
      {
        return 1;
      }

	int A, B;
	int moveA[4];
	int moveB[4];
	vector<string> allMoves;
	string moves1;
	string moves2;
	int xbut1 = 0;
	int ybut1 = 0;
	int xbut2 = 0;
	int ybut2 = 0;

	// Inform user to choose
	message7 = TTF_RenderText_Solid(font, "Player 1: Choose", textColor);
	apply_surface( 0, 0, beige, screen );
	apply_surface( 0, 0, choices, screen );
	apply_surface( 280, 370, message7, screen );
	if( SDL_Flip( screen ) == -1 )
	  {
	    return 1;
	  }

	// Choose first pokemon
	while(1){ 
	  if( SDL_PollEvent( &event ) ) {
	    if( event.type == SDL_MOUSEBUTTONDOWN ) {
	      xbut1 = event.button.x;
	      ybut1 = event.button.y;
	      A = ((ybut1/40)*16) + ((xbut1/37.5) + 1);
	      break;
	    }
	  }
	}
	SDL_Quit();

	// Instantiate first player's pokemon
	Pokemon user1 = Pokemon(A);
        user1.LoadMoves(0, rand()%162 + 1);
        user1.LoadMoves(1, rand()%162 + 1);
        user1.LoadMoves(2, rand()%162 + 1);
        user1.LoadMoves(3, rand()%162 + 1);
	
	/*
        // Inform user to choose                                                                                         
        message7 = TTF_RenderText_Solid(font, "Player 2: Choose", textColor);
	apply_surface( 0, 0, choices, screen );
        apply_surface( 280, 370, message7, screen );
	if( SDL_Flip( screen ) == -1 )
          {
            return 1;
          }

	// Choose second pokemon
	while(1) {
	  if( SDL_PollEvent( &event ) ) {
	    if( event.type == SDL_MOUSEBUTTONDOWN ) {
	      xbut2 = event.button.x;
	      ybut2 = event.button.y;
	      B = ((ybut2/40)*16) + ((xbut2/37.5) + 1);
	      break;
	    }
	  }
	}
	*/
       
	// Make sure ash in correct position
	int pos = 1;

	while(user1.getStatus() != fainted) {

	B = rand()%149 + 1;

	// Run the Veridian Forest
	pos = go(pos);

	//Initialize                                                                
	if( init() == false )
	  {
	    return 1;
	  }
	//Load the files                                                            
	if( load_files() == false )
	  {
	    return 1;
	  }

	// Create Battle Screen
	backClips(A);
	frontClips(B);
	message6 = TTF_RenderText_Solid(font2, "# MOVES #", textColor);
	apply_surface( 0, 0, background, screen );
	apply_surface( 360, 15, frontSprites, screen, &clips );
	apply_surface( 120, 174, backSprites, screen, &clips2 );
	apply_surface( 25, 295, message6, screen);
	

	//Update Screen
	if( SDL_Flip( screen ) == -1 )
	  {
	    return 1;
	  }


	//	Pokemon user1 = Pokemon(A);
	Pokemon user2 = Pokemon(B);

	//	user1.LoadMoves(0, 49);
	//	user1.LoadMoves(1, 77);
	//	user1.LoadMoves(2, 89);
	//	user1.LoadMoves(3, 110);

	user2.LoadMoves(0, rand()%162 + 1);
	user2.LoadMoves(1, rand()%162 + 1);
	user2.LoadMoves(2, rand()%162 + 1);
	user2.LoadMoves(3, rand()%162 + 1);

	int turn = 2;
	int move = 5;
    
	if (user1.getSpeed() > user2.getSpeed())
	  turn = 1;

	while (user1.getStatus() != fainted && user2.getStatus() != fainted) {


		

		switch(turn)
		{
			case 1:
				system("clear");

				// Display battlescreen
				message1 = TTF_RenderText_Solid(font2, user1.battleDisp().c_str(), textColor);
				message2 = TTF_RenderText_Solid(font, user2.battleDisp().c_str(), textColor);
      				apply_surface( 0, 0, background, screen );
				apply_surface( 120, 174, backSprites, screen, &clips2 );
				apply_surface( 360, 15, frontSprites, screen, &clips );
				apply_surface( 25, 295, message6, screen);
				apply_surface( 308, 257, message1, screen );
				apply_surface( 0, 5, message2, screen );

				//Update Screen
				if( SDL_Flip( screen ) == -1 )
				  {
				    return 1;
				  }

				// display the available moves
				allMoves = user1.disp_moves();
				moves1 = allMoves[0] + allMoves[1] + allMoves[2];
				moves2 = allMoves[3] + allMoves[4] + allMoves[5];
				message4 = TTF_RenderText_Solid(font2, moves1.c_str(), textColor);
				message5 = TTF_RenderText_Solid(font2, moves2.c_str(), textColor);
				apply_surface( 25, 310, message4, screen);
				apply_surface( 25, 330, message5, screen);
			        
			        // update screen
				if( SDL_Flip( screen ) == -1 )
				  {
				    return 1;
				  }

				//If there's an event to handle
				while( quit == false ) {
				  if( SDL_PollEvent( &event ) )
				    {
				      //If a key was pressed
				      if( event.type == SDL_KEYDOWN )
					{
					  switch( event.key.keysym.sym )
					    {
					    case SDLK_0: move = 0; break;
					    case SDLK_1: move = 1; break;
					    case SDLK_2: move = 2; break;
					    case SDLK_3: move = 3; break;
					    case SDLK_4: exit(1);
					    }
					}
				    }

				  if(move != 5) {

   				    // display the result
				    message3 = TTF_RenderText_Solid(font2, user1.useMove(move,user2).c_str(), textColor);
				    apply_surface( 25, 360, message3, screen );
                                
				    // update screen
				    if( SDL_Flip( screen ) == -1 )
				      {
					return 1;
				      }
				            
   				    sleep(3);
				    turn = 2;
				    break;
				  }
				}
		
				move = 5;
				break;
                
			 case 2:
				system("clear");

				// Display battlescreen and pokemon
				message1 = TTF_RenderText_Solid(font2, user1.battleDisp().c_str(), textColor);
				message2 = TTF_RenderText_Solid(font, user2.battleDisp().c_str(), textColor);
				apply_surface( 0, 0, background, screen );
				apply_surface( 120, 174, backSprites, screen, &clips2 );
				apply_surface( 360, 15, frontSprites, screen, &clips );
				apply_surface( 25, 295, message6, screen);
				apply_surface( 308, 257, message1, screen );
				apply_surface( 0, 5, message2, screen );
                
				//Update Screen
				if( SDL_Flip( screen ) == -1 )
				  {
				    return 1;
				  }
				/*
				// Display all moves
                                allMoves = user2.disp_moves();
                                moves1 = allMoves[0] + allMoves[1] + allMoves[2];
                                moves2 = allMoves[3] + allMoves[4] + allMoves[5];
				message4 = TTF_RenderText_Solid(font2, moves1.c_str(), textColor);
				message5 = TTF_RenderText_Solid(font2, moves2.c_str(), textColor);
				apply_surface( 25, 310, message4, screen);
				apply_surface( 25, 330, message5, screen);

				// update screen
				if( SDL_Flip( screen ) == -1 )
				  {
				    return 1;
				   }
				*/				
				
				//If there's an event to handle
				while( quit == false) {
				  if( SDL_PollEvent( &event ) )
				    {
				      //If a key was pressed
				      if( event.type == SDL_KEYDOWN )
					{
					  /*  switch( event.key.keysym.sym )
					    {
					    case SDLK_0: move = 0; break;
					    case SDLK_1: move = 1; break;
					    case SDLK_2: move = 2; break;
					    case SDLK_3: move = 3; break;
					    }*/
					  move = rand()%3;
					}
  				    }
				  if (move != 5 ) {

				    // Display result
				    message3 = TTF_RenderText_Solid(font2, user2.useMove(move,user1).c_str(), textColor);
				    apply_surface( 25, 360, message3, screen );
                                
				    // update screen
				    if( SDL_Flip( screen ) == -1 )
				      {
					return 1;
				      }
				                
				    sleep(3);
				    turn = 1;
				    break;
				  }
				}
				move = 5;
				break;
		}
	}
	SDL_Quit();
	}
}
