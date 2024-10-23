#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL2/SDL_pixels.h>
#include <stdbool.h>

// Window
#define WIDTH  800
#define HEIGHT 800

#define FONT_NAME "./FantasqueSansMono-Regular.ttf"

// Grid
// FIXME: GRID_MIN_x makes the last cells in row and columns empty
#define GRID_MIN_WIDTH  0
#define GRID_MIN_HEIGHT 0
#define GRID_MAX_WIDTH  WIDTH
#define GRID_MAX_HEIGHT HEIGHT
#define CELL_SIZE       20
#define TOTAL_ROWS      (GRID_MAX_HEIGHT / CELL_SIZE)
#define TOTAL_COLUMNS   (GRID_MAX_WIDTH / CELL_SIZE)
#define TOTAL_CELLS     (TOTAL_COLUMNS * TOTAL_ROWS)

// Colors: Saturation: 70, Value: 90
#define BACKGROUND_COLOR 32, 32, 32, 255
#define GRID_COLOR       28, 28, 28, 255
#define SNAKE_COLOR      93, 230, 69, 255
#define SNAKE_TAIL_COLOR 72, 179, 54, 255
#define FOOD_COLOR       230, 69, 74, 255

typedef enum
{
    RUNNING,
    GAME_OVER,
    QUIT
} State;

typedef struct
{
    int row;
    int column;
} Position;

typedef struct
{
    int x, y;
} Cell;

typedef struct
{
    Cell arr[TOTAL_CELLS];
    int size;
} Grid;

typedef struct Tail
{
    Position pos;
    struct Tail *next;
} Tail;

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct
{
    Position head_pos;
    Direction direction;
    int score;
    Tail *tail;
} Snake;

typedef struct
{
    Position pos;
} Food;

typedef struct
{
    Grid grid;
    Snake snake;
    Food food;
    State state;
    bool debug;
    int delay;
} Game;

#endif // GLOBAL_H
