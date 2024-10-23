#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "global.h"

void render_grid(SDL_Renderer *ren, Game *game)
{
    for (int i = 0; i < game->grid.size; ++i)
    {
        Position pos = {
            game->grid.arr[i].y / CELL_SIZE,
            game->grid.arr[i].x / CELL_SIZE,
        };

        SDL_Rect rect = {
            .x = game->grid.arr[i].x,
            .y = game->grid.arr[i].y,
            .w = CELL_SIZE,
            .h = CELL_SIZE};

        if (pos.row == game->snake.head_pos.row ||
            pos.column == game->snake.head_pos.column)
        {
            SDL_Color color = {GRID_COLOR};
            int b           = 10;

            SDL_SetRenderDrawColor(
                ren, color.r + b, color.g + b, color.b + b, color.a
            );
        }
        else
            SDL_SetRenderDrawColor(ren, GRID_COLOR);

        SDL_RenderDrawRect(ren, &rect);
    }
}

void render_tail(SDL_Renderer *ren, Grid *grid, Tail *tail, int depth)
{
    Cell cell = grid->arr[tail->pos.row * TOTAL_ROWS + tail->pos.column];

    SDL_Rect rect = {
        .x = cell.x,
        .y = cell.y,
        .w = CELL_SIZE,
        .h = CELL_SIZE,
    };

    SDL_SetRenderDrawColor(ren, SNAKE_TAIL_COLOR);
    SDL_RenderFillRect(ren, &rect);

    if (tail->next != NULL)
        render_tail(ren, grid, tail->next, depth + 1);
}

void render_snake(SDL_Renderer *ren, Game *game)
{
    int row    = game->snake.head_pos.row;
    int column = game->snake.head_pos.column;

    Cell head_cell = game->grid.arr[row * TOTAL_ROWS + column];

    SDL_Rect rect = {
        .x = head_cell.x,
        .y = head_cell.y,
        .w = CELL_SIZE,
        .h = CELL_SIZE,
    };

    if (game->snake.tail != NULL)
        render_tail(ren, &game->grid, game->snake.tail, 1);

    SDL_SetRenderDrawColor(ren, SNAKE_COLOR);
    SDL_RenderFillRect(ren, &rect);
}

void render_text_info(SDL_Renderer *ren, TTF_Font *font, Game *game)
{
    int x = 0;

    SDL_Color color = {255, 255, 255, 255};

    char *score_text = malloc(50 * sizeof(char));
    sprintf(score_text, "score: %i", game->snake.score);

    SDL_Surface *score_surface =
        TTF_RenderText_Blended(font, score_text, color);
    SDL_Texture *score_texture =
        SDL_CreateTextureFromSurface(ren, score_surface);

    SDL_Rect score_rect = {
        .x = x, .y = 0, .w = score_surface->w, .h = score_surface->h};

    x += score_rect.w + 20;

    SDL_RenderCopy(ren, score_texture, NULL, &score_rect);

    SDL_SetRenderDrawColor(ren, BACKGROUND_COLOR);
    SDL_RenderDrawRect(ren, &score_rect);

    free(score_surface);
    SDL_DestroyTexture(score_texture);
    free(score_text);

    // --------------------

    char *delay_text = malloc(50 * sizeof(char));
    sprintf(delay_text, "delay: %i", game->delay);

    SDL_Surface *delay_surface =
        TTF_RenderText_Blended(font, delay_text, color);
    SDL_Texture *delay_texture =
        SDL_CreateTextureFromSurface(ren, delay_surface);

    SDL_Rect delay_rect = {
        .x = x, .y = 0, .w = delay_surface->w, .h = delay_surface->h};

    x += delay_rect.w + 20;

    SDL_RenderCopy(ren, delay_texture, NULL, &delay_rect);

    SDL_SetRenderDrawColor(ren, BACKGROUND_COLOR);
    SDL_RenderDrawRect(ren, &delay_rect);

    free(delay_surface);
    SDL_DestroyTexture(delay_texture);
    free(delay_text);

    // --------------------

    char *debug_text = malloc(50 * sizeof(char));
    sprintf(debug_text, "debug: %s", game->debug == 0 ? "false" : "true");

    SDL_Surface *debug_surface =
        TTF_RenderText_Blended(font, debug_text, color);
    SDL_Texture *debug_texture =
        SDL_CreateTextureFromSurface(ren, debug_surface);

    SDL_Rect debug_rect = {
        .x = x, .y = 0, .w = debug_surface->w, .h = debug_surface->h};

    x += debug_rect.w + 20;

    SDL_RenderCopy(ren, debug_texture, NULL, &debug_rect);

    SDL_SetRenderDrawColor(ren, BACKGROUND_COLOR);
    SDL_RenderDrawRect(ren, &debug_rect);

    free(debug_surface);
    SDL_DestroyTexture(debug_texture);
    free(debug_text);
}

void render_food(SDL_Renderer *ren, Game *game)
{
    SDL_SetRenderDrawColor(ren, FOOD_COLOR);

    int row    = game->food.pos.row;
    int column = game->food.pos.column;

    Cell head_cell = game->grid.arr[row * TOTAL_ROWS + column];

    SDL_Rect rect = {
        .x = head_cell.x,
        .y = head_cell.y,
        .w = CELL_SIZE,
        .h = CELL_SIZE,
    };

    SDL_RenderFillRect(ren, &rect);
}
