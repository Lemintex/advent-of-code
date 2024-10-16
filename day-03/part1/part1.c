#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 2048
#define MAP_SIZE 140

typedef struct {
  int x;
  int y;
} symbol_t;

bool is_touching_symbol(int x, int y);
void check_map();

char map[MAP_SIZE][MAP_SIZE] = {0};
symbol_t symbols[MAX] = {0};

bool is_touching_symbol(int x, int y) {
  for (int x_index = x - 1; x_index <= x + 1; x_index++) {
    for (int y_index = y - 1; y_index <= y + 1; y_index++) {
      if (x_index == x && y_index == y)
        continue;
      if (x_index < 0 || x_index >= MAP_SIZE || y_index < 0 || y_index >= MAP_SIZE)
        continue;
      char c = map[y_index][x_index];
      if (c != '.' && !(c >= '0' && c <= '9')) {
        return true;
      }
    }
  }
  return false;
}

void check_map() {
  int total = 0;
  for (int y = 0; y < MAP_SIZE; y++) {
    char *pos = map[y];
    char *last_pos = map[y + 1];
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
      bool touching_symbol = false;
      for (int pos_x = x; pos_x < x + length; pos_x++) {
        touching_symbol = is_touching_symbol(pos_x, y);
        if (touching_symbol)
          break;
      }
      if (touching_symbol) {
        total += n;
      }
      if (last_digit >= last_pos - 1) {
        break;
      }
    }
  }
  printf("Total: %d", total);
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  char line[256];

  int y = 0;
  for (int i = 0; fgets(line, 256, input) != NULL; i++) {
    memcpy(&map[i], line, MAP_SIZE);
  }

  check_map();
}
