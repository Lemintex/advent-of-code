#include <math.h>
#include <stdbool.h>
#include <stdio.h>

int card_numbers[211][10] = {0};
int winning_numbers[211][25] = {0};
int number_of_cards[211] = {0};

void parse_line(char *line, int card_index) {
  int index = 10; // skip the first 10 characters
  for (int i = 0; i < 10; i++) {
    char c = line[index];
    if (line[index] >= '0' && line[index] <= '9') {
      card_numbers[card_index][i] *= 10;
      card_numbers[card_index][i] += line[index] - '0';
    }
    index++;
    c = line[index];
    if (line[index] >= '0' && line[index] <= '9') {
      card_numbers[card_index][i] *= 10;
      card_numbers[card_index][i] += line[index] - '0';
    }

    index += 2;
  }
  index += 2; // skip the "or"
  char c = line[index];
  for (int i = 0; i < 25; i++) {
    c = line[index];
    if (line[index] >= '0' && line[index] <= '9') {
      winning_numbers[card_index][i] *= 10;
      winning_numbers[card_index][i] += line[index] - '0';
    }
    index++;
    c = line[index];
    if (line[index] >= '0' && line[index] <= '9') {
      winning_numbers[card_index][i] *= 10;
      winning_numbers[card_index][i] += line[index] - '0';
    }
    index += 2;
  }
}

void return_game_total_that_win() {
  int total = 0;
  for (int i = 0; i < 211; i++) {
    number_of_cards[i] = 1;
  }
  for (int i = 0; i < 211; i++) {
    int winning_power = 0;
    for (int j = 0; j < 10; j++) {
      for (int k = 0; k < 25; k++) {
        int n1 = card_numbers[i][j];
        int n2 = winning_numbers[i][k];

        if (card_numbers[i][j] == winning_numbers[i][k]) {
          winning_power++;
          break;
        }
      }
    }
    // calculate the score
    int n = number_of_cards[i];
    for (int c = 1; c <= winning_power; c++) {
      if (i + c >= 211) {
        break;
      }
      number_of_cards[i + c] += n;
    }
    total += n;
  }
  printf("total: %d\n", total);
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];
  int i = 0;

  while (fgets(line, sizeof(line), input)) {
    parse_line(line, i);
    i++;
    /* code */
  }
  return_game_total_that_win();
}
