#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 992;
const int SCREEN_HEIGHT = 1070;
const int SCREEN_BPP = 32;

//The frames per second
const int FRAMES_PER_SECOND = 10;

//The dimenstions of the Ash
const int ASH_WIDTH = 21;
const int ASH_HEIGHT = 28;

//The direction status of Ash
const int ASH_RIGHT = 0;
const int ASH_LEFT = 1;
const int ASH_UP = 2;
const int ASH_DN = 3;

//The surfaces
SDL_Surface *ash = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;

//The event structure
SDL_Event event;

//The areas of the sprite sheet
SDL_Rect clipsRight[ 3 ];
SDL_Rect clipsLeft[ 3 ];
SDL_Rect clipsUp[ 3 ];
SDL_Rect clipsDn[ 3 ];

//Ash class
class Ash
{
  private:
    //The offset
    int xoffSet;
    int yoffSet;

    //Its rate of movement
    int xvelocity;
    int yvelocity;    

    //Its current frame
    int frame;

    //Its animation status
    int status;

  public:
    //Initializes the variables
    Ash();

    //Handles input
    void handle_events();

    //Moves Ash
    void move();

    //Shows Ash
    void show();
};

//The timer
class Timer
{
  private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

  public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

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
 
	if( optimizedImage != NULL ) {
	  //Map the color key
	  Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xFF, 0xFF, 0xFF );
	  SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey);
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

void set_clips()
{
    //Clip the sprites
    clipsRight[ 0 ].x = 47;
    clipsRight[ 0 ].y = 3;
    clipsRight[ 0 ].w = ASH_WIDTH;
    clipsRight[ 0 ].h = ASH_HEIGHT;

    clipsRight[ 1 ].x = 47;
    clipsRight[ 1 ].y = ASH_HEIGHT;
    clipsRight[ 1 ].w = ASH_WIDTH;
    clipsRight[ 1 ].h = ASH_HEIGHT;

    clipsRight[ 2 ].x = 47;
    clipsRight[ 2 ].y = ASH_HEIGHT * 2;
    clipsRight[ 2 ].w = ASH_WIDTH;
    clipsRight[ 2 ].h = ASH_HEIGHT;

    clipsLeft[ 0 ].x = 27;
    clipsLeft[ 0 ].y = 3;
    clipsLeft[ 0 ].w = ASH_WIDTH;
    clipsLeft[ 0 ].h = ASH_HEIGHT;

    clipsLeft[ 1 ].x = 27;
    clipsLeft[ 1 ].y = ASH_HEIGHT;
    clipsLeft[ 1 ].w = ASH_WIDTH;
    clipsLeft[ 1 ].h = ASH_HEIGHT;

    clipsLeft[ 2 ].x = 27;
    clipsLeft[ 2 ].y = ASH_HEIGHT * 2;
    clipsLeft[ 2 ].w = ASH_WIDTH;
    clipsLeft[ 2 ].h = ASH_HEIGHT;

    clipsUp[ 0 ].x = 68;  
    clipsUp[ 0 ].y = 3;
    clipsUp[ 0 ].w = ASH_WIDTH;
    clipsUp[ 0 ].h = ASH_HEIGHT;

    clipsUp[ 1 ].x = 68;
    clipsUp[ 1 ].y = ASH_HEIGHT;
    clipsUp[ 1 ].w = ASH_WIDTH;
    clipsUp[ 1 ].h = ASH_HEIGHT;

    clipsUp[ 2 ].x = 68;
    clipsUp[ 2 ].y = ASH_HEIGHT * 2;
    clipsUp[ 2 ].w = ASH_WIDTH;
    clipsUp[ 2 ].h = ASH_HEIGHT;

    clipsDn[ 0 ].x = 6;
    clipsDn[ 0 ].y = 3;
    clipsDn[ 0 ].w = ASH_WIDTH;
    clipsDn[ 0 ].h = ASH_HEIGHT;

    clipsDn[ 1 ].x = 6;
    clipsDn[ 1 ].y = ASH_HEIGHT;
    clipsDn[ 1 ].w = ASH_WIDTH;
    clipsDn[ 1 ].h = ASH_HEIGHT;

    clipsDn[ 2 ].x = 6;
    clipsDn[ 2 ].y = ASH_HEIGHT * 2;
    clipsDn[ 2 ].w = ASH_WIDTH;
    clipsDn[ 2 ].h = ASH_HEIGHT;

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

    //Set the window caption
    SDL_WM_SetCaption( "Pokemon Test", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the sprite sheet
    ash = load_image( "ash_ketchum.png" );
    
    //If there was a problem in loading the sprite
    if( ash == NULL )
    {
        return false;
    }

    //Load the background
    background = load_image( "VeridianForest.png" );

    //If there was a problem loading
    if( background == NULL )
      {
	return false;
      }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( ash );
    SDL_FreeSurface( background );

    //Quit SDL
    SDL_Quit();
}

Ash::Ash()
{
    //Initialize movement variables
    xoffSet = 0;
    yoffSet = 0;
    xvelocity = 0;
    yvelocity = 0;

    //Initialize animation variables
    frame = 0;
    status = ASH_RIGHT;
}

void Ash::handle_events()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Set the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_RIGHT: xvelocity += ASH_WIDTH / 3; break;
            case SDLK_LEFT: xvelocity -= ASH_WIDTH / 3; break;
	    case SDLK_UP: yvelocity -= ASH_WIDTH / 3; break;
	    case SDLK_DOWN: yvelocity += ASH_WIDTH / 3; break;

        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Set the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_RIGHT: xvelocity -= ASH_WIDTH / 3; break;
            case SDLK_LEFT: xvelocity += ASH_WIDTH / 3; break;
	    case SDLK_UP: yvelocity += ASH_WIDTH / 3; break;
	    case SDLK_DOWN: yvelocity -= ASH_WIDTH / 3; break;
        }
    }
}

void Ash::move()
{
    //Move
    xoffSet += xvelocity;
    yoffSet += yvelocity;

    //Keep the stick figure in bounds
    if( ( xoffSet < 0 ) || ( xoffSet + ASH_WIDTH > SCREEN_WIDTH ) )
    {
        xoffSet -= xvelocity;
    }
    if((yoffSet < 0) || (yoffSet + ASH_HEIGHT > SCREEN_HEIGHT)) {
      yoffSet -= yvelocity;
    }
}

void Ash::show()
{
    //Set the animations to the appropriate direction, move to next frame
    if( xvelocity < 0 )
    {
        status = ASH_LEFT;
        frame++;
    }
    else if( xvelocity > 0 )
      {
        status = ASH_RIGHT;
        frame++;
    }
    else if( yvelocity < 0 ) {
        status = ASH_UP;
        frame++;
    }
    else if( yvelocity > 0 ) {
        status = ASH_DN;
        frame++;
    }
    else
    {
        //Restart the animation
        frame = 0;
    }

    //Loop the animation
    if( frame >= 3 )
    {
        frame = 0;
    }

    //Show Ash
    if( status == ASH_RIGHT )
    {
        apply_surface(0, 0, background, screen);
        apply_surface( xoffSet, yoffSet, ash, screen, &clipsRight[ frame ] );
    }
    else if( status == ASH_LEFT )
    {
        apply_surface(0, 0, background, screen);
        apply_surface( xoffSet, yoffSet, ash, screen, &clipsLeft[ frame ] );
    }
    else if( status == ASH_UP ) {
        apply_surface(0, 0, background, screen);
        apply_surface( xoffSet, yoffSet, ash, screen, &clipsUp[ frame ] );
    }
    else if( status == ASH_DN ) {
        apply_surface(0, 0, background, screen);
        apply_surface( xoffSet, yoffSet, ash, screen, &clipsDn[ frame ] );
    }
}

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

int main( int argc, char* args[] )
{
    //Quit flag
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

    //Clip the sprite sheet
    set_clips();

    //The frame rate regulator
    Timer fps;

    //Ash Ketchum
    Ash test;

    //While the user hasn't quit
    while( quit == false )
    {
        //Start the frame timer
        fps.start();

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //Handle events for the stick figure
            test.handle_events();

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        //Move Ash
        test.move();

        //Show Ash on the screen
        test.show();

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

    //Clean up
    clean_up();

    return 0;
}
