#include <SDL2/SDL.h>
#include <stdio.h>

#include "global.h"

void update_snake_position(Snake *snake)
{
    switch (snake->direction)
    {
        case UP:
            {
                int row = snake->head_pos.row;

                if (row == 0)
                    snake->head_pos.row = TOTAL_ROWS - 1;
                else
                    snake->head_pos.row = row - 1;
            }
            break;
        case DOWN:
            {
                int row = snake->head_pos.row;

                if (row == TOTAL_ROWS - 1)
                    snake->head_pos.row = 0;
                else
                    snake->head_pos.row = row + 1;
            }
            break;
        case LEFT:
            {
                int column = snake->head_pos.column;

                if (column == 0)
                    snake->head_pos.column = TOTAL_COLUMNS - 1;
                else
                    snake->head_pos.column = column - 1;
            }
            break;
        case RIGHT:
            {
                int column = snake->head_pos.column;

                if (column == TOTAL_COLUMNS - 1)
                    snake->head_pos.column = 0;
                else
                    snake->head_pos.column = column + 1;
            }
            break;
    }

    // Update tails position
    Tail *current          = snake->tail;
    Position prev_position = snake->head_pos;

    while (current != NULL)
    {
        Position temp_position = current->pos;
        current->pos           = prev_position;
        prev_position          = temp_position;

        current = current->next;
    }
}

void generate_food(Game *game)
{
    int row    = rand() % ((TOTAL_ROWS - 1) + 0);
    int column = rand() % ((TOTAL_COLUMNS - 1) + 0);

    Food food = {
        .pos = {.row = row, .column = column}
    };

    game->food = food;
}

void new_tail(Game *game, Tail **head)
{
    Tail **current = head;

    while ((*current)->next != NULL)
        current = &(*current)->next;

    (*current)->next             = malloc(sizeof(Tail));
    (*current)->next->pos.row    = game->snake.head_pos.row;
    (*current)->next->pos.column = game->snake.head_pos.column;
    (*current)->next->next       = NULL;
}

void check_food_snake_intersection(Game *game)
{
    Position food_pos = game->food.pos;
    Cell food_cell =
        game->grid.arr[food_pos.row * TOTAL_ROWS + food_pos.column];

    SDL_Rect food = {
        .x = food_cell.x,
        .y = food_cell.y,
        .w = CELL_SIZE,
        .h = CELL_SIZE,

    };

    Position snake_head_pos = game->snake.head_pos;
    Cell snake_head_cell =
        game->grid.arr[snake_head_pos.row * TOTAL_ROWS + snake_head_pos.column];

    SDL_Rect snake_head = {
        .x = snake_head_cell.x,
        .y = snake_head_cell.y,
        .w = CELL_SIZE,
        .h = CELL_SIZE,
    };

    if (SDL_HasIntersection(&snake_head, &food))
    {
        generate_food(game);
        game->snake.score++;

        if (game->snake.tail == NULL)
        {
            game->snake.tail             = malloc(sizeof(Tail));
            game->snake.tail->pos.row    = game->snake.head_pos.row;
            game->snake.tail->pos.column = game->snake.head_pos.column;
            game->snake.tail->next       = NULL;
        }
        else
            new_tail(game, &game->snake.tail);
    }
}
