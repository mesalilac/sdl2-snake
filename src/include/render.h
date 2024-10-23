#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "global.h"

void render_grid(SDL_Renderer *ren, Game *game);
void render_snake(SDL_Renderer *ren, Game *game);
void render_text_info(SDL_Renderer *ren, TTF_Font *font, Game *game);
void render_food(SDL_Renderer *ren, Game *game);

#endif // RENDER_H
