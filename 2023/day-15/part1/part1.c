#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_hash(char *l) {
  int result = 0;
  int hash = 0;
  for (int i = 0; i < strlen(l); i++) {
    char c = l[i];
    if (c == '\n') {
      continue;
    }
    if (l[i] == ',') {
      result += hash;
      hash = 0;
      continue;
    }
    hash += l[i];
    hash *= 17;
    hash %= 256;
  }
  result += hash;
  return result;
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[200000] = {0};

  int total = 0;
  while (fgets(line, sizeof(line), input)) {
    total = run_hash(line);
  }
  printf("Total: %d\n", total);
}
