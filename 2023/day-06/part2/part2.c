#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RACES 4
typedef struct {
  long int time;
  long int distance_to_beat;
} race_t;

race_t race = {0};

void calculate_time() {
  int ways_to_win = 0;
  int min = 0, max = 0;
  for (int t = 0; t < race.time; t++) {
    long int speed = t;
    long int time_to_move = race.time - t;
    long int distance = time_to_move * speed;
    if (distance > race.distance_to_beat) {
      min = t;
      printf("Min: %d\n", min);
      break;
    }
  }
  for (int t = race.time - 1; t > 0; t--) {
    long int speed = t;
    long int time_to_move = race.time - t;
    long int distance = time_to_move * speed;
    if (distance > race.distance_to_beat) {
      max = t;
      printf("Max: %d\n", max);
      break;
    }
  }
  printf("Way to win: %d\n", max - min + 1);
}

void parse_input(char *line_contents, int line_number) {
  if (line_number == 0) {
    char *first_digit = strpbrk(line_contents, "0123456789");
    if (first_digit == NULL)
      return;
    char *last_digit = first_digit + strspn(first_digit, "0123456789");
    int length = last_digit - first_digit;
    char number[20] = {0};
    strncpy(number, first_digit, length);
    race.time = atoi(number);
    line_contents = last_digit;
  } else if (line_number == 1) {
    char *first_digit = strpbrk(line_contents, "0123456789");
    if (first_digit == NULL)
      return;
    char *last_digit = first_digit + strspn(first_digit, "0123456789");
    int length = last_digit - first_digit;
    char number[20] = {0};
    strncpy(number, first_digit, length);
    race.distance_to_beat = atol(number);
    line_contents = last_digit;
  }
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];
  int l = 0;
  int i = 0;
  while (fgets(line, sizeof(line), input)) {
    char parsed[256] = {0};
    for (int i = 0, j = 0; i < 256; i++) {
      if (line[i] >= '0' && line[i] <= '9') {
        parsed[j] = line[i];
        j++;
      }
      parsed[i] = line[i];
    }
    parse_input(parsed, l++);
  }
  calculate_time();
}
