// An empity window painted in red that stay alive for 9001ms and exits
// gcc -lSDL2 -lfreetype -I/usr/include/freetype2 clen_window_text_freetype.c
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <stdio.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;

int main(int argc, char *args[]) {
  FT_Library ft2;
  FT_Face fc;
  unsigned int glyph;
  int err;
  err = FT_Init_FreeType(&ft2);
  if (err)
    printf("There was an error on FT_Init_FreeType."
           "Who knows what happened!!\n\tUse fterrdef.h to find them out");

  err = FT_New_Face(
      ft2, "/usr/share/fonts/adobe-source-sans-pro/SourceSansPro-Regular.otf",
      0, &fc);
  if (err)
    printf("Error on FT_New_Face\n");
  err = FT_Set_Char_Size(fc, 0, 16 * 64, 300, 300);
  if (err)
    printf("Error on FT_Set_Char_Size\n");
  glyph = FT_Get_Char_Index(fc, 'G');
  err = FT_Load_Glyph(fc, glyph, FT_LOAD_DEFAULT); // fc->glyph->bitmap
  if (err)
    printf("error!\n");
  err = FT_Render_Glyph(fc->glyph, FT_RENDER_MODE_NORMAL);
  if (err)
    printf("Error FT_Render_Glyph: %d\n", err);

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
    SDL_Rect rec;
    rec.x = 10;
    rec.y = 10;
    rec.w = (int)((FT_GlyphSlot)fc->glyph)->advance.x >> 6;
    rec.h = (int)((FT_GlyphSlot)fc->glyph)->advance.y >> 6;
    texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        (int)fc->glyph->bitmap.width, (int)fc->glyph->bitmap.rows);
    SDL_RenderClear(renderer);
    // Setting the blend mode is importante to make the alpha channel work!!!
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    {
      void *p;
      int pitch;
      SDL_LockTexture(texture, NULL, &p, &pitch);
      unsigned int *pixels = (unsigned int *)p;
      SDL_PixelFormat *pf = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
      FT_Bitmap *bitmap = &fc->glyph->bitmap;
      unsigned char *buff = bitmap->buffer;
      for (unsigned int y = 0; y < bitmap->rows; ++y) {
        // printf("%.2d:", y);
        for (unsigned int x = 0; x < bitmap->width; ++x) {
          unsigned int index = y * bitmap->width + x;
          // if(buff[index] == 0) printf("    ");
          // else printf(" %.3d", buff[index]);
          pixels[index] = SDL_MapRGBA(pf, 0xFF, 0xFF, 0xFF, buff[index]);
        }
        //printf("\n");
        unsigned int pixel = pixels[15 * bitmap->width + 1];
      }
      SDL_FreeFormat(pf);
      SDL_UnlockTexture(texture);
    }

    SDL_Rect quad;
    quad.x = 10;
    quad.y = 10;
    // for next positioning after the last glyph
    // quad.x = (int)(0 + (fc->glyph->metrics.horiBearingX >> 6));
    // quad.y = (int)(0 + (fc->glyph->metrics.horiBearingY >>6));
    SDL_QueryTexture(texture, NULL, NULL, &quad.w, &quad.h);

    SDL_RenderCopy(renderer, texture, NULL, &quad);
    SDL_DestroyTexture(texture);

    SDL_SetRenderTarget(renderer, NULL);
  }
  int quit = 0;
  // screenSurface = SDL_GetWindowSurface(window);
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT)
        quit = 1;
    }
    SDL_RenderPresent(renderer);
    // SDL_FillRect( screenSurface, NULL, SDL_MapRGB(screenSurface->format,
    // 0xFF,0x10,0x00));
    // SDL_UpdateWindowSurface(window);
  }

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
