#include <exception>
#include <string>
#include <iostream>
#include <SDL.h>

#include "../src/shared/scrollBar.h"
#include "../src/shared/keys.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;

class InitError : public std::exception
{
    std::string msg;
public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};

InitError::InitError() :
    exception(),
    msg( SDL_GetError() )
{
}

InitError::InitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

InitError::~InitError() throw()
{
}

const char * InitError::what() const throw()
{
    return msg.c_str();
}

class SDL
{
    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
    SCROLLBAR m_vert, m_horz;
public:
    SDL( Uint32 flags = 0 );
    virtual ~SDL();
    void draw();
};

SDL::SDL( Uint32 flags )
{
    if ( SDL_Init( flags ) != 0 )
        throw InitError();

    if ( SDL_CreateWindowAndRenderer( 1024, 768, SDL_WINDOW_SHOWN,
                                      &m_window, &m_renderer ) != 0 )
        throw InitError();

    g_window = m_window;
    g_renderer = m_renderer;

    scroll_init();
    scroll_setBarThickness(41, TRUE);

    scrollBar_init(&m_vert, SCROLL_VERTICAL);
    scrollBar_setRect(&m_vert, 580, 10, 50, 500);
    scrollBar_setMaxLength(&m_vert, 100);
    scrollBar_setLength(&m_vert, 18);
    scrollBar_moveTo(&m_vert, 80);

    scrollBar_init(&m_horz, SCROLL_HORIZONTAL);
    scrollBar_setRect(&m_horz, 10, 580, 500, 50);
    scrollBar_setMaxLength(&m_horz, 100);
    scrollBar_setLength(&m_horz, 75);
    scrollBar_moveTo(&m_horz, 12);
}

SDL::~SDL()
{
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    SDL_Quit();
}

void SDL::draw()
{
    // Clear the window with a white background
    SDL_SetRenderDrawColor( m_renderer, 255, 255, 255, 255 );
    SDL_RenderClear( m_renderer );

    // Show the window
    SDL_RenderPresent( m_renderer );

    int rgb[] = { 203, 203, 203, // Gray
                  254, 254,  31, // Yellow
                    0, 255, 255, // Cyan
                    0, 254,  30, // Green
                  255,  16, 253, // Magenta
                  253,   3,   2, // Red
                   18,  14, 252, // Blue
                    0,   0,   0  // Black
                };

    SDL_Rect colorBar;
    colorBar.x = 0; colorBar.y = 0; colorBar.w = 90; colorBar.h = 480;

    // Render a new color bar every 0.5 seconds
    for ( int i = 0; i != sizeof rgb / sizeof *rgb; i += 3, colorBar.x += 90 )
    {
        SDL_SetRenderDrawColor( m_renderer, rgb[i], rgb[i + 1], rgb[i + 2], 255 );
        SDL_RenderFillRect( m_renderer, &colorBar );

        scrollBar_drawEx(&m_vert, FALSE, TRUE);
        scrollBar_drawEx(&m_horz, FALSE, TRUE);

        SDL_RenderPresent( m_renderer );
        SDL_Delay( 500 );
    }
}

int main( int argc, char * argv[] )
{
    try
    {
        SDL sdl( SDL_INIT_VIDEO | SDL_INIT_TIMER );

         sdl.draw();

         uint car(KEY_NONE);
         while (car == KEY_NONE){
            car = getKey();
        }

        return 0;
    }
    catch ( const InitError & err )
    {
        std::cerr << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;
    }

    return 1;
}
