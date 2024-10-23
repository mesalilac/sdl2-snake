#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "include/global.h"
#include "include/logic.h"
#include "include/render.h"

void fill_grid(Grid *grid)
{
    grid->size = 0;
    for (int row = GRID_MIN_HEIGHT; row < GRID_MAX_HEIGHT; row += CELL_SIZE)
    {
        for (int col = GRID_MIN_WIDTH; col < GRID_MAX_WIDTH; col += CELL_SIZE)
        {
            grid->arr[grid->size] = (Cell){.x = col, .y = row};
            grid->size++;
        }
    }
}

void free_tail(Tail *tail)
{
    if (tail->next != NULL)
    {
        free_tail(tail->next);
    }

    tail = NULL;
    free(tail);
}

void reset_game(Game *game)
{
    game->snake.head_pos.row    = TOTAL_ROWS / 2;
    game->snake.head_pos.column = TOTAL_COLUMNS / 2;
    game->snake.direction       = UP;
    game->snake.score           = 0;

    if (game->snake.tail != NULL)
        free_tail(game->snake.tail);

    game->snake.tail = NULL;
    game->state      = RUNNING;
    game->debug      = false;
    game->delay      = 50;

    game->snake.tail->pos.row    = game->snake.head_pos.row;
    game->snake.tail->pos.column = game->snake.head_pos.column;
    game->snake.tail->next       = NULL;
}

int main()
{
    srand(time(0));

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(
            stderr, "ERROR: Failed to initalize sdl video: %s\n", SDL_GetError()
        );
        exit(1);
    }

    SDL_Window *win =
        SDL_CreateWindow("Snake", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL)
    {
        fprintf(
            stderr, "ERROR: Failed to create renderer: %s\n", SDL_GetError()
        );
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        fprintf(
            stderr, "ERROR: Failed to initalize sdl ttf: %s\n", SDL_GetError()
        );
        exit(1);
    }

    TTF_Font *font = TTF_OpenFont(FONT_NAME, 18);
    if (font == NULL)
    {
        fprintf(stderr, "ERROR: Failed to open font: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Event event;

    Game game = {
        .grid = {.size = 0},
        .snake =
            {.head_pos  = {.row = TOTAL_ROWS / 2, .column = TOTAL_COLUMNS / 2},
                 .direction = UP,
                 .score     = 0,
                 .tail      = malloc(sizeof(Tail))},
        .state = RUNNING,
        .debug = true,
        .delay = 50,
    };

    fill_grid(&game.grid);

    game.snake.tail->pos.row    = game.snake.head_pos.row;
    game.snake.tail->pos.column = game.snake.head_pos.column;
    game.snake.tail->next       = NULL;

    generate_food(&game);

    while (game.state != QUIT)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    game.state = QUIT;
                    break;
                case SDL_KEYDOWN:
                    {
                        int key = event.key.keysym.sym;
                        if (key == 'r')
                            reset_game(&game);
                        else if (key == 'w' && game.snake.direction != DOWN)
                            game.snake.direction = UP;
                        else if (key == 's' && game.snake.direction != UP)
                            game.snake.direction = DOWN;
                        else if (key == 'a' && game.snake.direction != RIGHT)
                            game.snake.direction = LEFT;
                        else if (key == 'd' && game.snake.direction != LEFT)
                            game.snake.direction = RIGHT;
                        else if (key == '-' && game.delay > 10)
                            game.delay -= 10;
                        else if (key == '=')
                            game.delay += 10;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (event.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
                    {
                        Cell head_cell =
                            game.grid.arr
                                [game.snake.head_pos.row * TOTAL_ROWS +
                                 game.snake.head_pos.column];

                        int hx = head_cell.x;
                        int hy = head_cell.y;

                        printf(
                            "x: %i, y: %i | hx: %i, hy: %i\n",
                            event.button.x,
                            event.button.y,
                            hx,
                            hy
                        );

                        if (event.button.x <= hx)
                            game.snake.direction = LEFT;

                        if (event.button.x >= hx)
                            game.snake.direction = RIGHT;

                        if (event.button.y <= hy)
                            game.snake.direction = UP;

                        if (event.button.y >= hy)
                            game.snake.direction = DOWN;
                    }
                    break;
            }
        }

        update_snake_position(&game.snake);
        check_food_snake_intersection(&game);

        SDL_SetRenderDrawColor(ren, BACKGROUND_COLOR);
        SDL_RenderClear(ren);

        render_snake(ren, &game);
        render_food(ren, &game);
        render_grid(ren, &game);
        render_text_info(ren, font, &game);

        SDL_RenderPresent(ren);
        SDL_Delay(game.delay);
    }

    TTF_CloseFont(font);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
