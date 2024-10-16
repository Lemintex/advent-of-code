#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 746
typedef struct point {
  long int x;
  long int y;
} point_t;

point_t points[LENGTH] = {0};

int main() {
  int min_x = 0, min_y = 0;
  int x = 0, y = 0;

  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];
  int point_index = 0;
  long int perimeter = 0;
  while (fgets(line, sizeof(line), input)) {
    int length = 0;
    char *c = strchr(line, '#');
    c++;
    for (int i = 0; i < 5; i++) {
      int num = 0;
      if (c[i] >= '0' && c[i] <= '9') {
        num = c[i] - '0';
      } else if (c[i] >= 'a' && c[i] <= 'f') {
        num = c[i] - 'a' + 10;
      }
      length *= 16;
      length += num;
    }
    perimeter += length;
    switch (c[5]) {
    case '0': // right
      x += length;
      break;
    case '2': // left
      x -= length;
      break;
    case '3': // up
      y -= length;
      break;
    case '1': // down
      y += length;
      break;
    }
    points[point_index++] = (point_t){x, y};

    if (x < min_x)
      min_x = x;
    if (y < min_y)
      min_y = y;
  }

  // we need to ensure that all points are positive for gausses area formula
  for (int i = 0; i < LENGTH; i++) {

    points[i].x += abs(min_x);
    points[i].y += abs(min_y);
  }

  long int area = 0;
  for (int i = 0; i < LENGTH; i++) {
    area -= points[i].x * points[(i + 1) % LENGTH].y;
    area += points[i].y * points[(i + 1) % LENGTH].x;
  }
  area = labs(area) / 2;
  long int interior = area - perimeter / 2 + 1;
  printf("Total area: %ld\n", interior + perimeter);
  return 0;
}
