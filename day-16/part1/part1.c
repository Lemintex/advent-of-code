#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { NORTH, EAST, SOUTH, WEST } direction_t;

typedef struct {
  char tile;
  bool visited;
} tile_t;

tile_t **map;
int map_width = 0, map_height = 0;

void update_xy(int *x, int *y, direction_t direction) {
  switch (direction) {
  case NORTH:
    (*y)--;
    break;
  case EAST:
    (*x)++;
    break;
  case SOUTH:
    (*y)++;
    break;
  case WEST:
    (*x)--;
    break;
  }
}

void step(int x, int y, direction_t direction) {
  if (x < 0 || x >= map_width || y < 0 || y >= map_height) {
    return;
  }

  char tile = map[y][x].tile;

  if ((tile == '|' || tile == '-') && map[y][x].visited) {
    return;
  }

  map[y][x].visited = true;
  switch (tile) {
  case '/':
    switch (direction) {
    case NORTH:
      direction = EAST;
      break;

    case EAST:
      direction = NORTH;
      break;

    case SOUTH:
      direction = WEST;
      break;

    case WEST:
      direction = SOUTH;
      break;
    }
    update_xy(&x, &y, direction);
    step(x, y, direction);
    break;

  case '\\':
    switch (direction) {
    case NORTH:
      direction = WEST;
      break;

    case EAST:
      direction = SOUTH;
      break;

    case SOUTH:
      direction = EAST;
      break;

    case WEST:
      direction = NORTH;
      break;
    }
    update_xy(&x, &y, direction);
    step(x, y, direction);
    break;

  case '|':
    switch (direction) {
    case NORTH:
    case SOUTH:
      update_xy(&x, &y, direction);
      step(x, y, direction);
      break;

    case EAST:
    case WEST:
      int x1 = x, y1 = y;
      int x2 = x, y2 = y;
      update_xy(&x1, &y1, NORTH);
      update_xy(&x2, &y2, SOUTH);
      step(x1, y1, NORTH);
      step(x2, y2, SOUTH);
      return;
    }
    break;

  case '-':
    switch (direction) {
    case NORTH:
    case SOUTH:
      int x1 = x, y1 = y;
      int x2 = x, y2 = y;
      update_xy(&x1, &y1, EAST);
      update_xy(&x2, &y2, WEST);
      step(x1, y1, EAST);
      step(x2, y2, WEST);
      return;

    case EAST:
    case WEST:
      update_xy(&x, &y, direction);
      step(x, y, direction);
      break;
    }
    break;

  case '.':
    update_xy(&x, &y, direction);
    step(x, y, direction);
    break;
  }
}

void simulate() {
  direction_t direction = EAST;
  step(0, 0, direction);
}

int evaluate() {
  int total = 0;
  for (int i = 0; i < map_height; i++) {
    for (int j = 0; j < map_width; j++) {
      if (map[i][j].visited) {
        total++;
      }
    }
  }
  return total;
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  while (fgets(line, sizeof(line), input)) {
    map_height++;
  }
  rewind(input);

  // allocate the memory for the map
  map = (tile_t **)malloc(sizeof(tile_t *) * map_height);

  for (int i = 0; fgets(line, sizeof(line), input); i++) {
    map[i] = (tile_t *)malloc(sizeof(tile_t) * strlen(line));
    for (int j = 0; j < strlen(line); j++) {
      map[i][j].visited = false;
      map[i][j].tile = line[j];
    }
  }

  map_width = strlen(line);

  for (int i = 0; i < map_height; i++) {
    for (int j = 0; j < map_width; j++) {
      char tile = map[i][j].tile;
    }
  }

  simulate();
  printf("Total: %d\n", evaluate());
}
