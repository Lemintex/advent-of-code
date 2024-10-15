#include <stdio.h>
#include <stdbool.h>

int process_line(char *line);

int main() {
  FILE *input = fopen("../input.txt", "r");

  int sum = 0;

  // big char array to hold the line
  char line[256];

  while (fgets(line, sizeof(line), input)) {
    sum += process_line(line);
  }
  printf("%d\n", sum);
}

int process_line(char *line) {
  int start = 0;
  int end = 0;

  bool found = false;
  for (int i = 0; line[i] != '\0'; i++) {
    // check if the current character is a number
    if (line[i] >= '0' && line[i] <= '9') {
      if (!found) {
        start = line[i] - '0';
        found = true;
      }
      end = line[i] - '0';
    }
  }
  return start * 10 + end;
}
