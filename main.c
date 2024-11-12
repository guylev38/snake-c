#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include "snake.h"

#define FPS (5)
#define FRAME_TIME_MS (1000 / FPS)

long get_current_time_ms(){
  return (long)time(NULL)*1000;
}

void handle_fps(long start_time){
  long end_time, elapsed_time, sleep_time;
  end_time = get_current_time_ms();
  elapsed_time = end_time - start_time;

  sleep_time = FRAME_TIME_MS - elapsed_time;
  if(sleep_time > 0){
    usleep(sleep_time * 1000);
  }
}

void init_colors(){
  init_pair(1, COLOR_GREEN, COLOR_BLACK); // Green - Snake
  init_pair(2, COLOR_RED, COLOR_BLACK); // Red - Apple
  init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Wall - Apple

}

int main(void){
  char** grid = init_grid();
  Snake* snake = malloc(sizeof(Snake));
  Apple* apple = malloc(sizeof(Apple));
  int score = 0;
  int i;

  init_snake(snake);

  grid[snake->y[0]][snake->x[0]] = SNAKE;
  spawn_apple(grid, apple);

  // Init ncurses 
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  srand(time(NULL));

  if(has_colors() == FALSE){
    endwin();
    printf("Your terminal doesn't support colors!\n");
    exit(1);
  }

  start_color();
  init_colors();

  printw("Press any key to start...");

  // Game Loop
  int ch;
  long start_time;

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

    // Check if snake hit wall
    if(check_hit_wall(snake) == TRUE){
      printw("Game Over! You hit a wall\n");      
      refresh();
      break; 
    } 

    // check if snake hit himself
    if(check_hit_self(snake) == TRUE){
      printw("Game Over! You hit yourself!\n");
      refresh();
      break;
    }

    // Check if the snake eaten the apple
    if(snake->x[0] == apple->x && snake->y[0] == apple->y){
      ++score;
      grow_snake(snake);
      spawn_apple(grid, apple);
    }

    // Handle input
    switch(ch){
      case 'w': snake->dir = UP; break;
      case 's': snake->dir = DOWN; break;
      case 'a': snake->dir = LEFT; break;
      case 'd': snake->dir = RIGHT; break;
    }

    move_player(snake, grid);
    draw_grid(grid);

    refresh();
    handle_fps(start_time);
  }

  printw("Quitting Snake...\n");      
  printw("Score: %d", score);
  refresh();
  usleep(2000000);
  endwin(); // quit ncurses

  // Memory cleanup
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