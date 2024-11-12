#ifndef SNAKE_H
#define SNAKE_H

#define ROWS (20)
#define COLS (20)

#define CELL ('.')
#define SNAKE ('X')
#define WALL ('*')
#define APPLE ('#')

typedef enum Direction { 
  UP,
  LEFT,
  RIGHT,
  DOWN,
  NONE
} Direction;

typedef struct Snake{
  int length;
  int* x;
  int* y;
  Direction dir;
} Snake;

typedef struct Apple{
  int x;
  int y;
} Apple;

void draw_grid(char **grid);
char** init_grid();
void spawn_apple(char **grid, Apple *apple);
void move_player(Snake *snake, char **grid);
void grow_snake(Snake *snake);
int check_hit_wall(Snake *snake);
int check_hit_self(Snake *snake);
void init_snake(Snake *snake);

#endif