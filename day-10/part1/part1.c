#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAP_SIZE 139

enum direction { UP, DOWN, LEFT, RIGHT };

typedef struct {
  char pipe;
  enum direction incoming_direction[2];
  enum direction outgoing_direction[2];
} pipe_type_t;

typedef struct {
  bool visited[MAP_SIZE][MAP_SIZE];
  char map[MAP_SIZE][MAP_SIZE + 1]; // +1 for null terminator
} pipe_map_t;

pipe_type_t pipes[6] = {
    {'|', {UP, DOWN}, {UP, DOWN}},    {'-', {LEFT, RIGHT}, {LEFT, RIGHT}},
    {'L', {DOWN, LEFT}, {RIGHT, UP}}, {'J', {DOWN, RIGHT}, {LEFT, UP}},
    {'7', {UP, RIGHT}, {LEFT, DOWN}}, {'F', {UP, LEFT}, {RIGHT, DOWN}}};

pipe_map_t pipe_map = {0};
int total_steps = 0;

void move_to_next_pipe(int x, int y, enum direction *moving_direction);
bool is_valid_pipe(char pipe, enum direction *direction);

void start(int start_x, int start_y) {
  total_steps = 0;
  enum direction moving_direction = DOWN;
  move_to_next_pipe(start_x, start_y, &moving_direction);
  if (total_steps == 0) {
    moving_direction = UP;
    move_to_next_pipe(start_x, start_y, &moving_direction);
  }
  if (total_steps == 0) {
    moving_direction = LEFT;
    move_to_next_pipe(start_x, start_y, &moving_direction);
  }
  if (total_steps == 0) {
    moving_direction = RIGHT;
    move_to_next_pipe(start_x, start_y, &moving_direction);
  }
  total_steps++;
}

void move_to_next_pipe(int x, int y, enum direction *moving_direction) {
  bool valid_pipe_found = false;
  bool visited = pipe_map.visited[y][x];
  char pipe;
  if (x < MAP_SIZE - 1) {
    pipe = pipe_map.map[y][x + 1];
    if (*moving_direction == RIGHT && is_valid_pipe(pipe, moving_direction)) {
      valid_pipe_found = true;
      pipe_map.visited[y][x] = true;
      move_to_next_pipe(x + 1, y, moving_direction);
    }
  }
  if (x > 0) {
    pipe = pipe_map.map[y][x - 1];
    if (*moving_direction == LEFT && is_valid_pipe(pipe, moving_direction)) {
      valid_pipe_found = true;
      pipe_map.visited[y][x] = true;
      move_to_next_pipe(x - 1, y, moving_direction);
    }
  }
  if (y < MAP_SIZE - 1) {
    pipe = pipe_map.map[y + 1][x];
    if (*moving_direction == DOWN && is_valid_pipe(pipe, moving_direction)) {
      valid_pipe_found = true;
      pipe_map.visited[y][x] = true;
      move_to_next_pipe(x, y + 1, moving_direction);
    }
  }
  if (y > 0) {
    pipe = pipe_map.map[y - 1][x];
    if (*moving_direction == UP && is_valid_pipe(pipe, moving_direction)) {
      valid_pipe_found = true;
      pipe_map.visited[y][x] = true;
      move_to_next_pipe(x, y - 1, moving_direction);
    }
  }
}

bool is_valid_pipe(char pipe, enum direction *direction) {
  for (int i = 0; i < 6; i++) {
    if (pipes[i].pipe == pipe) {
      for (int j = 0; j < 2; j++) {
        if (pipes[i].incoming_direction[j] == *direction) {
          total_steps++;
          *direction = pipes[i].outgoing_direction[j];
          return true;
        }
      }
    }
  }
  return false;
}

int main() {
  int x = 0, y = 0;
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  bool start_found = false;
  for (int i = 0; fgets(line, sizeof(line), input); i++) {
    strcpy(pipe_map.map[i], line);
    if (start_found) {
      continue;
    }
    for (int j = 0; j < MAP_SIZE; j++) {
      if (pipe_map.map[i][j] == 'S') {
        x = j;
        y = i;
        start_found = true;
        break;
      }
    }
  }
  start(x, y);
  printf("Total steps: %d\n", total_steps / 2);
}
