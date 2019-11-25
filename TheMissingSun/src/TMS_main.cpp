#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>

/* Quit all SDL subsystems. */
void quit();

int main(int argc, char* args[])
{
    /* Initialise SDL library, exit on failure. */
    /* Init SDL's video, audio and timer subsystems. */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
    {
        printf("%s", SDL_GetError());
        quit();
        return -1;
    }
    /* Init SDL_Image for png and jpg file loading. */
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

    quit();
    return 0;
}

void quit()
{
    Mix_Quit(); // Quit SDL_Mixer.
    TTF_Quit(); // Quit SDL_TTF.
    IMG_Quit(); // Quit SDL_Image.
    SDL_Quit(); // Quit SDL.
}