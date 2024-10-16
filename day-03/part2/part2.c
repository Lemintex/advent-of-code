#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 2048
#define MAP_SIZE 140
int gear_x = 0, gear_y = 0;

// can't have 3 gears touching each other
typedef struct {
  int x;
  int y;
  int parts[2];
  int gears; // rename
  bool is_valid;
} gears_t;

void add_to_new_gear(int n);
void add_to_existing_gear_info(int n, int index);
void check_existing_gear_info(int n);
bool is_touching_gear(int x, int y);
void calculate_gear_ratio();
void check_map();

char map[MAP_SIZE][MAP_SIZE] = {0};
gears_t gears[MAX];
int gears_count = -1;

void add_to_new_gear(int n) {
  gears_count++;
  gears[gears_count].x = gear_x;
  gears[gears_count].y = gear_y;
  gears[gears_count].parts[0] = n;
  gears[gears_count].gears = 1;
  gears[gears_count].is_valid = true;
}

void add_to_existing_gear_info(int n, int index) {
  gears[index].parts[1] = n;
  gears[index].gears = 2;
}

void check_existing_gear_info(int n) {
  for (int i = 0; i <= gears_count; i++) {
    if (gears[i].x == gear_x && gears[i].y == gear_y) {
      if (gears[i].gears == 2 && gears[i].is_valid) {
        gears[i].is_valid = false;
      } else {
        add_to_existing_gear_info(n, i);
      }
      return;
    }
  }
  add_to_new_gear(n);
}

bool is_touching_gear(int x, int y) {
  for (int x_index = x - 1; x_index <= x + 1; x_index++) {
    for (int y_index = y - 1; y_index <= y + 1; y_index++) {
      if (x_index == x && y_index == y)
        continue;
      if (x_index < 0 || x_index >= MAP_SIZE || y_index < 0 ||
          y_index >= MAP_SIZE)
        continue;
      char c = map[y_index][x_index];
      if (c == '*') {
        gear_x = x_index;
        gear_y = y_index;
        return true;
      }
    }
  }
  return false;
}

void calculate_gear_ratio() {
  int total = 0;
  for (int i = 0; i <= gears_count; i++) {
    if (gears[i].is_valid && gears[i].gears == 2) {
      total += gears[i].parts[0] * gears[i].parts[1];
    }
  }
  printf("Total:%d", total);
}

void check_map() {
  for (int y = 0; y < MAP_SIZE; y++) {
    char *pos = map[y];
    char *lastPos = map[y + 1];
    while (true) {
      char *first_digit = strpbrk(pos, "0123456789");
      if (first_digit >= map[y + 1] || first_digit == NULL) {
        break;
      }

      char *last_digit = first_digit + strspn(first_digit, "0123456789");
      int length = 0;
      if (last_digit >= map[y + 1] || last_digit == NULL) {
        last_digit = map[y + 1] - 1;
        length = last_digit - first_digit;
        length++;
      } else {
        length = last_digit - first_digit;
      }

      char number[10] = {0};
      strncpy(number, first_digit, length);
      int n = atoi(number);
      pos = last_digit;

      if (n == 0)
        continue;
      int x = first_digit - map[y];
      bool is_touching = false;

      for (int posX = x; posX < x + length; posX++) {
        is_touching = is_touching_gear(posX, y);
        if (is_touching) {
          check_existing_gear_info(n);
        }
        if (is_touching)
          break;
      }
      if (last_digit >= lastPos - 1) {
        break;
      }
    }
  }
  calculate_gear_ratio();
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  char line[256];

  int y = 0;
  for (int i = 0; fgets(line, 256, input) != NULL; i++) {
    memcpy(&map[i], line, MAP_SIZE);
  }

  for (int i = 0; i < MAX; i++) {
    gears[i].x = 0;
    gears[i].y = 0;
    gears[i].parts[0] = 0;
    gears[i].parts[1] = 0;
    gears[i].gears = 0;
    gears[i].is_valid = false;
  }
  check_map();
}
