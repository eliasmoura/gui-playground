// An empity window painted in black with an outlined and a filled square. It
// stays stays alive until the user close the window
//
// gcc -lSDL2 -lcairo -I/usr/include/freetype2 clen_window_cairo.c
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <stdio.h>

#include <cairo/cairo.h>

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 600;

void render_square(SDL_Renderer *renderer, SDL_Texture *texture, cairo_t *cairo,
                   uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
  // Setting the blend mode is importante to make the alpha channel work!!!
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  void *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, &pixels, &pitch);
  cairo_surface_t *surf;
  surf = cairo_image_surface_create_for_data(
      pixels, CAIRO_FORMAT_ARGB32, SCREEN_WIDTH, SCREEN_HEIGHT, pitch);
  cairo = cairo_create(surf);
  cairo_set_line_width(cairo, 10.6);
  cairo_set_source_rgb(cairo, 1, 1, 0);
  cairo_rectangle(cairo, 10.05, 10.05, 100.5, 100.5);
  cairo_stroke(cairo);
  cairo_rectangle(cairo, 10.05, 10.05, 100.5, 100.5);
  cairo_set_source_rgb(cairo, 0, 1, 1);
  cairo_fill(cairo);

  SDL_UnlockTexture(texture);
}

int main(int argc, char *args[]) {
  SDL_Window *window = NULL;
  // SDL_Surface *screenSurface = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Texture *texture = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL not initialized! EROO: %s\n", SDL_GetError());
    return 1;
  }

  // create the window
  window = SDL_CreateWindow("SDL HelloWorld :)", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Windo not created! ERROR: %s\n", SDL_GetError());
    return 1;
  }

  {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
      printf("Failed: CreateRenderer → %s\n", SDL_GetError());
      return 1;
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    SDL_Rect quad;
    // int x = 0;
    int y = 100;
    quad.x = 10;
    quad.y = y;
    cairo_t *cr = NULL;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                SCREEN_HEIGHT);
    render_square(renderer, texture, cr, 0xFF, 0xFF, 0xFF, 0xFF);

    SDL_QueryTexture(texture, NULL, NULL, &quad.w, &quad.h);

    SDL_RenderCopy(renderer, texture, NULL, &quad);
    SDL_DestroyTexture(texture);
  }
  SDL_SetRenderTarget(renderer, NULL);
  int quit = 0;
  // screenSurface = SDL_GetWindowSurface(window);
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT)
        quit = 1;
    }
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
