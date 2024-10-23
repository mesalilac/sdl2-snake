#ifndef LOGIC_H
#define LOGIC_H

#include "global.h"

void update_snake_position(Snake *snake);
void generate_food(Game *game);
void check_food_snake_intersection(Game *game);

#endif // LOGIC_H
