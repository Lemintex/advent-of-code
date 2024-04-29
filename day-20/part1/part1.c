#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pulses[2] = {0, 0};

typedef enum module_type { BUTTON, RELAY, FLIPFLOP, CONJUNCTION } module_type_e;

typedef struct module {
  char state;
  char **modules;
  char type;
  char name[2];
} module_t;

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  while (fgets(line, sizeof(line), input)) {
    char *moudle_name = strtok(line, " ");
    char *module_type = strtok(NULL, " ");

    char *tok[16];
    char *token = strtok(NULL, ", ");
    int n = 0;
    while (token != NULL) {
      tok[n++] = token;
      token = strtok(NULL, ", ");
    }
    printf("%s %s\n", moudle_name, module_type);
    for (int i = 0; i < n; i++) {
      printf("%s\n", tok[i]);
    }
  }
}
