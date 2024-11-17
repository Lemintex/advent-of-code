#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

char **dish;
int dish_size = 0, line_length = 0;

void roll_dish_north() {
  int total = 0;
  for (int i = 0; i < dish_size; i++) // for each row
  {
    for (int j = 0; j < dish_size; j++) // for each column
    {
      int k = i;
      bool found = false;
      for (k = i; k > 0 && dish[k - 1][j] == '.' && dish[k][j] == 'O'; k--) {
        found = true;
        dish[k - 1][j] = 'O';
        dish[k][j] = '.';
      }
      if (found || dish[k][j] == 'O') {
        total += dish_size - k;
      }
    }
  }
  printf("Total: %d\n", total);
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  while (fgets(line, sizeof(line), input)) {
    dish_size++;
  }

  dish = malloc(dish_size * sizeof(char *));

  rewind(input);
  for (int i = 0; i < dish_size; i++) {
    dish[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
    fgets(dish[i], MAX_LINE_LENGTH, input);
  }
  line_length = strlen(dish[0]);
  roll_dish_north();
}
