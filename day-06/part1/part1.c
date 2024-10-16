#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RACES 4
typedef struct {
  int time;
  int distance_to_beat;
} race_t;

race_t races[4] = {0};

void calculate_time() {
  int total = 1;
  for (int i = 0; i < NUM_RACES; i++) {
    int ways_to_win = 0;
    for (int t = 0; t < races[i].time; t++) {
      int speed = t;
      int time_to_move = races[i].time - t;
      int distance = time_to_move * speed;
      if (distance > races[i].distance_to_beat) {
        ways_to_win++;
      }
    }
    printf("Way to win: %d\n", ways_to_win);
    total *= ways_to_win;
  }
  printf("Total ways to win: %d\n", total);
}

void parse_input(char *line_contents, int line_number) {
  if (line_number == 0) {
    for (int r = 0; r < NUM_RACES; r++) {
      char *first_digit = strpbrk(line_contents, "0123456789");
      if (first_digit == NULL)
        return;
      char *last_digit = first_digit + strspn(first_digit, "0123456789");
      int length = last_digit - first_digit;
      char number[10] = {0};
      strncpy(number, first_digit, length);
      races[r].time = atoi(number);
      line_contents = last_digit;
    }

  } else if (line_number == 1) {
    for (int r = 0; r < NUM_RACES; r++) {
      char *first_digit = strpbrk(line_contents, "0123456789");
      if (first_digit == NULL)
        return;
      char *last_digit = first_digit + strspn(first_digit, "0123456789");
      int length = last_digit - first_digit;
      char number[10] = {0};
      strncpy(number, first_digit, length);
      races[r].distance_to_beat = atoi(number);
      line_contents = last_digit;
    }
  }
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];
  int l = 0;
  int i = 0;
  while (fgets(line, sizeof(line), input)) {
    parse_input(line, l++);
  }
  calculate_time();
}
