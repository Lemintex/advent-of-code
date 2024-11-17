#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

char **dish;
int dish_size = 0, line_length = 0;

// working
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
    }
  }
}

// working
void roll_dish_east() {
  int total = 0;
  for (int i = line_length - 1; i >= 0; i--) // for each column
  {
    for (int j = 0; j < dish_size; j++) // for each row
    {
      int k = i;
      bool found = false;
      for (k = i;
           k < line_length - 1 && dish[j][k + 1] == '.' && dish[j][k] == 'O';
           k++) {
        found = true;
        dish[j][k + 1] = 'O';
        dish[j][k] = '.';
      }
    }
  }
}

// working
void roll_dish_south() {
  int total = 0;
  for (int i = dish_size - 1; i >= 0; i--) // for each row
  {
    for (int j = 0; j < line_length; j++) // for each column
    {
      int k = i;
      bool found = false;
      for (k = i;
           k < dish_size - 1 && dish[k + 1][j] == '.' && dish[k][j] == 'O';
           k++) {
        found = true;
        dish[k + 1][j] = 'O';
        dish[k][j] = '.';
      }
    }
  }
}

void roll_dish_west() {
  int total = 0;
  for (int i = 0; i < line_length; i++) // for each row
  {
    for (int j = 0; j < dish_size; j++) // for each column
    {
      int k = i;
      bool found = false;
      for (k = i; k > 0 && dish[j][k - 1] == '.' && dish[j][k] == 'O'; k--) {
        found = true;
        dish[j][k - 1] = 'O';
        dish[j][k] = '.';
      }
    }
  }
}

void roll_dish() {
  roll_dish_north();
  roll_dish_west();
  roll_dish_south();
  roll_dish_east();
}

int calculate_load() {
  int total = 0;
  for (int i = 0; i < dish_size; i++) {
    for (int j = 0; j < line_length; j++) {
      if (dish[i][j] == 'O') {
        total += dish_size - i;
      }
    }
  }
  return total;
}
int main() {
  int total = 0;
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

  int loop_count = 0;
  for (int i = 0; i < 1000; i++) {
    roll_dish();
  }
  int reference = calculate_load();
  do {
    roll_dish();
    loop_count++;
  } while (calculate_load() != reference);

  int mod = (1000000000 - 1000) % loop_count;

  for (int i = 0; i < mod; ++i) {
    roll_dish();
  }
  printf("Total: %d\n", calculate_load());

  for (int i = 0; i < dish_size; i++) {
    printf("%s", dish[i]);
    free(dish[i]);
  }
}
