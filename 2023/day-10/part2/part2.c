// There is an elusive off-by-one error somewhere in this code. I have no idea
// where it is, but I should probably find it. I THINK it's due to the fact that
// the last pipe in the loop is not counted as a tile inside the loop, but I'm
// not sure.
// TODO: find off-by-one error
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

int Count_tiles_inside_loop() {
  // FILE* output = fopen("../output.txt", "w");
  int count = 0;
  char pipe;
  for (int i = 0; i < MAP_SIZE; i++) {
    bool inside_loop = false;
    for (int j = 0; j < MAP_SIZE; j++) {
      char p = pipe_map.map[i][j];
      if (pipe_map.visited[i][j]) {
        printf("%c", pipe_map.map[i][j]);
        // fprintf(output, "%c", p);
        if (p == 'F' || p == '7' || p == '|') {
          inside_loop = !inside_loop;
        }
      } else {
        // fprintf(output, ".");
        if (inside_loop) {
          count++;
        }
      }
    }
    // fprintf(output, "\n");
    // printf("\n");
  }
  // fclose(output);
  return count;
}

void Start(int start_x, int start_y) {
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
  pipe_map.visited[start_y][start_x] = true;
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
      pipe_map.visited[i][j] = false;
      if (pipe_map.map[i][j] == 'S') {
        x = j;
        y = i;
        start_found = true;
        break;
      }
    }
  }
  Start(x, y);
  printf("Total steps: %d\n", total_steps / 2);
  printf("Tiles visited: %d\n", total_steps);
  printf("Tiles outside loop: %d\n", MAP_SIZE * MAP_SIZE - total_steps);
  printf("Tiles inside loop: %d\n", Count_tiles_inside_loop());
}
