#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_SIZE 410
char map[MAP_SIZE][MAP_SIZE];

int main() {
  int min_x = 0, min_y = 0;
  int max_x = 0, max_y = 0;
  int x = 0, y = 0;

  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  for (int i = 0; i < MAP_SIZE; i++) {
    for (int j = 0; j < MAP_SIZE; j++) {
      map[i][j] = '.';
    }
  }

  while (fgets(line, sizeof(line), input)) {
    switch (line[0]) {
    case 'R':
      x += atoi(&line[2]);
      break;

    case 'L':
      x -= atoi(&line[2]);
      break;

    case 'U':
      y -= atoi(&line[2]);
      break;

    case 'D':
      y += atoi(&line[2]);
      break;
    }
    if (x < min_x)
      min_x = x;
    if (y < min_y)
      min_y = y;
    if (x > max_x)
      max_x = x;
    if (y > max_y)
      max_y = y;
  }
  rewind(input);

  x = -min_x, y = -min_y;
  while (fgets(line, sizeof(line), input)) {
    switch (line[0]) {
    case 'R':
      for (int i = 0; i < atoi(&line[2]); i++) {
        x++;
        map[y][x] = '#';
      }
      break;

    case 'L':
      for (int i = 0; i < atoi(&line[2]); i++) {
        x--;
        map[y][x] = '#';
      }
      break;

    case 'U':
      for (int i = 0; i < atoi(&line[2]); i++) {
        y--;
        map[y][x] = '#';
      }
      break;

    case 'D':
      for (int i = 0; i < atoi(&line[2]); i++) {
        y++;
        map[y][x] = '#';
      }
      break;
    }
  }

  int total = 0;
  for (int i = 0; i < -min_y + max_y + 1; i++) {
    bool inside = false;

    for (int j = 0; j < -min_x + max_x + 1; j++) {
      if (map[i][j] == '#') {
        total++;
        if ((i > 0 && map[i - 1][j] == '#') &&
            (i < -min_x + max_x && map[i + 1][j] == '#')) {
          inside = !inside;
        } else if (i > 0 && map[i - 1][j] == '#') {
          // inside = false;
        } else if (i < -min_x + max_x && map[i + 1][j] == '#') {
          inside = !inside;
        }
      } else if (inside) {
        total++;
        map[i][j] = 'X';
      }
    }
  }
  fclose(input);
  // FILE *output = fopen("output.txt", "w");
  //
  // for (int i = 0; i < -min_y + max_y + 1; i++) {
  //   for (int j = 0; j < -min_x + max_x + 1; j++) {
  //     fprintf(output, "%c", map[i][j]);
  //   }
  //   fprintf(output, "\n");
  // }
  printf("Total: %d\n", total);
  return 0;
}
