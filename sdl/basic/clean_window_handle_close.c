// An empity window painted in red that stay alive while the user does not close it
// gcc -lSDL2 01_basic_window.c
#include <SDL2/SDL.h>
#include <stdio.h>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;

int main(int argc, char* args[])
{
    SDL_Window *window = NULL;

    SDL_Surface *screenSurface = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL not initialized! EROO: %s\n", SDL_GetError());
        return 1;
    }

    //create the window
    window = SDL_CreateWindow("SDL HelloWorld :)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    													SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        printf("Windo not created! ERROR: %s\n", SDL_GetError());
        return 1;
    }
		int quit = 0;
    screenSurface = SDL_GetWindowSurface(window);
		SDL_Event e;
    while(!quit){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT)
                quit = 1;
        }
        SDL_FillRect( screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF,0x10,0x00));
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
