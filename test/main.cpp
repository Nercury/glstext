#include <SDL.h>
#include <SDL/SDL_ttf.h>
#include "GLST_Text.h"

void init_window();
void quit_window();
void window_resize();
void draw_scene();

using namespace std;

#define SCREEN_BPP 16
int width;
int height;

bool done;

int main(int argc, char *argv[])
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        cout << "Failed to initialize SDL: " << SDL_GetError() << endl;
        return 1;
    } else {
        SDL_EnableUNICODE(true);
        cout << "SDL initialized." << endl;
    }

    if (TTF_Init() < 0)
    {
        cout << "TTF library initialization failed: " << TTF_GetError() << endl;
        return 1;
    }

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) /* | SDL_INIT_AUDIO */
    {
        cout << "Video initialization failed: " << SDL_GetError() << endl;
        return 1;
    }

    /* this holds some info about our display */
    const SDL_VideoInfo *videoInfo;

    /* Fetch the video info */
    videoInfo = SDL_GetVideoInfo();

    if ( !videoInfo )
    {
        cout << "Video query failed: " << SDL_GetError() << endl;
        return 1;
    }

    /* Flags to pass to SDL_SetVideoMode */
    int videoFlags;

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
        videoFlags |= SDL_HWSURFACE;
    else
        videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
        videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    width = 640;
    height = 480;

    /* get a SDL surface */
    SDL_Surface *surface = SDL_SetVideoMode( width, height, SCREEN_BPP,
                videoFlags );

    /* Verify there is a surface */
    if ( !surface )
    {
        cout << "Video mode set failed: " << SDL_GetError( ) << endl;
        return 1;
    }

    /* init window and resize it */
    init_window();
    window_resize();

    /* used to collect events */
    SDL_Event sdl_event;

    done = false;
    while (!done)
    {
        /* handle the events in the queue */
        while ( SDL_PollEvent( &sdl_event ) )
        {
            switch( sdl_event.type )
            {
            case SDL_VIDEORESIZE:
                /* handle resize event */
                width = sdl_event.resize.w;
                height = sdl_event.resize.h;

                surface = SDL_SetVideoMode( sdl_event.resize.w,
                            sdl_event.resize.h,
                            SCREEN_BPP, videoFlags );
                if ( !surface )
                {
                    cout << "Could not get a surface after resize: " << SDL_GetError() << endl;
                    done = true;
                }

                /* after resize, textures may need reinitialization */
                window_resize();

                break;
            case SDL_KEYDOWN:
                /* handle key presses here */
                if (sdl_event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;
                break;
            case SDL_QUIT:
                /* handle quit requests */
                done = true;
                break;
            default:
                break;
            }
        }

        if (!done)
            draw_scene();
    }

    quit_window();

    TTF_Quit();

    SDL_Quit();

    cout << "Done." << endl;
    return 0;
}

GLST_Text* text;
GLST_Text* text2;

void init_window()
{
    text = new GLST_Text("bin/FreeSerif.ttf", 50, GLST_Middle_Center);
    text2 = new GLST_Text("bin/FreeSerif.ttf", 30, GLST_Middle_Center);
}

void quit_window()
{
    delete text;
    delete text2;
}

void window_resize()
{
    glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    glOrtho( 0, (GLsizei)width, (GLsizei)height , 0, 0, 1 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    /* call to glst to check if textures are ok */
    GLST_Refresh(); /* <<<<<<< ! */
}

int frames = 0;
int update = 0;
int fps = 0;

string _to_string(int value)
{
    std::string s;
    std::stringstream out;
    out << value;
    return out.str();
}

void draw_scene()
{
    if ((SDL_GetTicks() - update) > 1000)
    {
        fps = (int)(frames / (float)(SDL_GetTicks()-update) * 1000);
        update = SDL_GetTicks();
        frames = 0;
    }
    frames++;

    /* draw nice background to demostrate text transparency */
    glBegin( GL_QUADS );
    glColor3f(   0.5f,  0.5f,  1.0f );glVertex2i(  width,  0 );glVertex2i(  0,  0 );
    glColor3f(   0.0f,  0.0f,  0.2f );glVertex2i(  0,  height );glVertex2i(  width, height );
    glEnd();

    /* enable textures and blending before drawing text */
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    /* call before rendering any text */
    GLST_SetSceneStart(); /* <<<<<<< ! */

    /* Draw dynamic text */
    /* SetText costs fps, try to set only when necessary */
    text->SetText("Hello world!");
    text->SetAlpha((SDL_GetTicks() - update) / 1000.0f);
    text->Render(width / 2, height / 2);

    text2->SetText(_to_string(frames) + " frame");
    text2->SetColor(1.0f, 0.8f, 0.7f);
    text2->Render(width / 2, height / 2 + text->Height());

    text2->SetText(_to_string(fps) + " fps");
    text2->SetColor(0.8f, 1.0f, 0.7f);
    text2->Render(width / 2, height / 2 - text->Height());

    /* you can create and destroy GLST text object in render method
     * without big performance hit, but it is *not* good */
    GLST_Text text3("bin/FreeSerif.ttf", 30, GLST_Middle_Center);
    text3.SetText("In loop!");
    text3.Render(width / 2, text3.Height());

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    /* Draw it to the screen */
    SDL_GL_SwapBuffers();
}
