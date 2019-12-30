/* Temporary main file for testing. */

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        printf("%s", IMG_GetError());
        quit();
        return -1;
    }
    /* Init SDL_TTF. */
    if (TTF_Init() == -1)
    {
        printf("%s", Mix_GetError());
        quit();
        return -1;
    }
    /* Init SDL_Mixer for mp3 file playback. */
    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3)
    {
        printf("%s", Mix_GetError());
        quit();
        return -1;
    }

    return 0;
}