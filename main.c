#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#define FPS (5)
#define FRAME_TIME_MS (1000 / FPS)

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

void draw_grid(char** grid){
  int max_row, max_col, start_row, start_col, i, j;
  getmaxyx(stdscr, max_row, max_col);

  start_row = (max_row - ROWS) / 2;
  start_col = (max_col - COLS) / 2;

  // Draw the grid
  for (i = 0; i < ROWS; i++) {
    for (j = 0; j < COLS; j++) {
      mvaddch(start_row + i, start_col + j, grid[i][j]);
    }
  }
}

char** init_grid(){
  char** grid = (char**)malloc(ROWS*sizeof(char*));
  int i,j; 

  for(i = 0; i < ROWS; i++){
    grid[i] = (char*)malloc(COLS*sizeof(char));
  }

  for(i = 0; i < ROWS; i++){
    for(j = 0; j < COLS; j++){
      if(i == 0 || j == 0 || i == (ROWS-1) || j == (COLS-1)) {
        grid[i][j] = WALL;
      }
      else {
        grid[i][j] = CELL;
      }
    }
  } 
  return grid;
}

long get_current_time_ms(){
  return (long)time(NULL)*1000;
}

void spawn_apple(char** grid, Apple* apple){
  int rx, ry;

  do{
    rx = (rand() % (COLS - 2)) + 1;
    ry = (rand() % (ROWS - 2)) + 1;
  } while(grid[ry][rx] != CELL);

  grid[ry][rx] = APPLE;
  apple->y = ry;
  apple->x = rx;
}

void move_player(Snake* snake, char** grid){
  // Shift the body
  int i;
  int slen = snake->length;

  grid[snake->y[slen-1]][snake->x[slen-1]] = CELL; 
  for(i = slen - 1; i > 0; i--){
    snake->x[i] = snake->x[i - 1];
    snake->y[i] = snake->y[i - 1];
  }

  // Move the head based on the direction
  switch(snake->dir){
    case UP: 
      snake->y[0]--;
      break;
    case DOWN:
      snake->y[0]++;
      break;
    case LEFT: 
      snake->x[0]--;
      break;
    case RIGHT:
      snake->x[0]++;
      break;
    default:
      break;
  }
  

  // Update the new position of the head
  grid[snake->y[0]][snake->x[0]] = SNAKE; 
}

void grow_snake(Snake* snake){
  snake->x[snake->length] = snake->x[snake->length - 1];
  snake->y[snake->length] = snake->y[snake->length - 1];
  snake->length++;
}

int check_hit_wall(Snake* snake){
  if(snake->x[0] <= 0 || snake->x[0] >= COLS - 1 || snake->y[0] <= 0 || snake->y[0] >= ROWS - 1){
    return TRUE;
  }
  return FALSE;
}

int check_hit_self(Snake* snake, char** grid){
  int i;
  for(i = snake->length-1; i>0; i--){
    if(snake->x[0] == snake->x[i] && snake->y[0] == snake->y[i]){
      return TRUE;
    }
  }
  return FALSE;
}

int main(void){
  char** grid = init_grid();
  Snake* snake = malloc(sizeof(Snake));
  snake->x = malloc(100 * sizeof(int));
  snake->y = malloc(100 * sizeof(int));
  Apple* apple = malloc(sizeof(Apple));
  int score = 0;
  int i;

  snake->length = 1;
  snake->x[0] = COLS/2;
  snake->y[0] = ROWS/2;
  snake->dir = NONE;

  grid[snake->y[0]][snake->x[0]] = SNAKE;
  spawn_apple(grid, apple);

  // Init ncurses 
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  srand(time(NULL));

  printw("Press any key to start...");

  // Game Loop
  int ch;
  long start_time, end_time, elapsed_time, sleep_time;

  while((ch = getch()) != 'q'){
    start_time = get_current_time_ms();

    cbreak();
    noecho();
    curs_set(0);
    timeout(0);
    nodelay(stdscr, TRUE);

    // Game Logic and Rendering
    clear();
    printw("Score %d\n", score);
    if(check_hit_wall(snake) == TRUE){
      printw("Game Over! You hit a wall\n");      
      refresh();
      break; 
    } 
    if(check_hit_self(snake, grid) == TRUE){
      printw("Game Over! You hit yourself!\n");
      refresh();
      break;
    }

    if(snake->x[0] == apple->x && snake->y[0] == apple->y){
      ++score;
      grow_snake(snake);
      spawn_apple(grid, apple);
    }

    switch(ch){
      case 'w': snake->dir = UP; break;
      case 's': snake->dir = DOWN; break;
      case 'a': snake->dir = LEFT; break;
      case 'd': snake->dir = RIGHT; break;
    }

    move_player(snake, grid);

    draw_grid(grid);

    refresh();

    end_time = get_current_time_ms();
    elapsed_time = end_time - start_time;

    sleep_time = FRAME_TIME_MS - elapsed_time;
    if(sleep_time > 0){
      usleep(sleep_time * 1000);
    }
  }
  // clear();
  printw("Quitting Snake...\n");      
  printw("Score: %d", score);
  refresh();
  usleep(1000000);
  endwin(); // quit ncurses

  for(i = 0; i < ROWS; i++){
    free(grid[i]);
  }
  free(grid);
  free(snake->x);
  free(snake->y);
  free(snake);
  free(apple);

  return 0;
}