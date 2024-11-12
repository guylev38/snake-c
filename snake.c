#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "snake.h"

void draw_grid(char **grid){
  int max_row, max_col, start_row, start_col, i, j;
  getmaxyx(stdscr, max_row, max_col);

  start_row = (max_row - ROWS) / 2;
  start_col = (max_col - COLS) / 2;

  // Draw the grid
  for (i = 0; i < ROWS; i++) {
    for (j = 0; j < COLS; j++) {
      if(grid[i][j] == SNAKE){
        attron(COLOR_PAIR(1));
        mvaddch(start_row + i, start_col + j, grid[i][j]);
        attroff(COLOR_PAIR(1));
      }
      else if(grid[i][j] == APPLE){
        attron(COLOR_PAIR(2));
        mvaddch(start_row + i, start_col + j, grid[i][j]);
        attroff(COLOR_PAIR(2));
      }
      else if(grid[i][j] == WALL){
        attron(COLOR_PAIR(3));
        mvaddch(start_row + i, start_col + j, grid[i][j]);
        attroff(COLOR_PAIR(3));
      } 
      else{
        mvaddch(start_row + i, start_col + j, grid[i][j]);
      }
    }
  }
}

char** init_grid(){
  char **grid = (char**)malloc(ROWS*sizeof(char*));
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

void spawn_apple(char **grid, Apple *apple){
  int rx, ry;

  do{
    rx = (rand() % (COLS - 2)) + 1;
    ry = (rand() % (ROWS - 2)) + 1;
  } while(grid[ry][rx] != CELL);

  grid[ry][rx] = APPLE;
  apple->y = ry;
  apple->x = rx;
}

void move_player(Snake *snake, char **grid){
  int i;
  int slen = snake->length;

  // Clear the old position of the snake
  grid[snake->y[slen-1]][snake->x[slen-1]] = CELL; 
  // Shift the body
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

void grow_snake(Snake *snake){
  snake->x[snake->length] = snake->x[snake->length - 1];
  snake->y[snake->length] = snake->y[snake->length - 1];
  snake->length++;
}

int check_hit_wall(Snake *snake){
  if(snake->x[0] <= 0 || snake->x[0] >= COLS - 1 || snake->y[0] <= 0 || snake->y[0] >= ROWS - 1){
    return TRUE;
  }
  return FALSE;
}

int check_hit_self(Snake *snake){
  int i;
  for(i = snake->length-1; i>0; i--){
    if(snake->x[0] == snake->x[i] && snake->y[0] == snake->y[i]){
      return TRUE;
    }
  }
  return FALSE;
}

void init_snake(Snake *snake){
  snake->x = malloc(100 * sizeof(int));
  snake->y = malloc(100 * sizeof(int));
  snake->length = 1;
  snake->x[0] = COLS / 2;
  snake->y[0] = ROWS / 2;
  snake->dir = NONE;
}