#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#define TARGET_FPS (60)
#define FRAME_TIME_MS (1000 / TARGET_FPS)

#define ROWS (20)
#define COLS (15)

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

typedef struct Player{
  int x;
  int y;
  Direction dir;
} Player;

typedef struct Apple{
  int x;
  int y;
} Apple;

void draw_grid(char** grid){

  int max_row, max_col, start_row, start_col, i, j;
  getmaxyx(stdscr, max_row, max_col);

  start_row = (max_row - ROWS) / 2; 
  start_col = (max_col - COLS) / 2;

  for(i = 0; i < ROWS; i++){
    for(j = 0; j < COLS; j++){
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
  srand(time(NULL));
  int rrow, rcol;

  do{
    rrow = (rand() % (ROWS - 2)) + 1;
    rcol = (rand() % (COLS - 2)) + 1;
  } while(grid[rrow][rcol] == SNAKE  || grid[rrow][rcol] == WALL);


  grid[rrow][rcol] = APPLE;
  apple->x = rrow;
  apple->y = rcol;
}

void move_player(Player* player, char** grid){
  grid[player->y][player->x] = CELL;
  switch(player->dir){
    case UP:
      player->y--;
      break;
    case DOWN:
      player->y++;
      break;
    case LEFT:
      player->x--;
      break;
    case RIGHT:
      player->x++;
      break;
    default:
      break;
  }

  grid[player->y][player->x] = SNAKE;
}


int main(void){
  char** grid = init_grid();
  Player* player = malloc(sizeof(Player));
  Apple* apple = malloc(sizeof(Apple));
  int score = 0;

  player->x = ROWS/2;
  player->y = COLS/2;
  player->dir = NONE;

  grid[player->y][player->x] = SNAKE;
  spawn_apple(grid, apple);

  // Init ncurses 
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  printw("Press any key to start...");

  // Game Loop
  int ch;
  long start_time, end_time, elapsed_time, sleep_time;
  while((ch = getch()) != 'q'){
    start_time = get_current_time_ms();

    cbreak();
    noecho();
    curs_set(0);

    // Game Logic and Rendering
    clear();

    if(ch == 'w') player->dir = UP;
    else if(ch == 'a') player->dir = LEFT;
    else if(ch == 's') player->dir = DOWN;
    else if(ch == 'd') player->dir = RIGHT;

    move_player(player, grid);

    if(player->x == apple->x && player->y == apple->y){
      ++score;
      spawn_apple(grid, apple);
    }

    draw_grid(grid);

    refresh();

    end_time = get_current_time_ms();
    elapsed_time = end_time - start_time;

    sleep_time = FRAME_TIME_MS - elapsed_time;
    if(sleep_time > 0){
      usleep(sleep_time * 1000);
    }
    if(grid[player->y][player->x] == WALL){
      printw("Game Over! You hit a wall\n");
      printw("Score: %d", score);
      refresh();
      usleep(2 * 1000000);
      endwin();
      exit(0);
    }

  }
  endwin(); // quit ncurses
  return 0;
}