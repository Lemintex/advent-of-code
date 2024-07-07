#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pulses[2] = {0, 0};

typedef enum module_type { FLIPFLOP, CONJUNCTION, BROADCAST } module_type_e;
typedef enum pulse { LOW, HIGH } pulse_e;

typedef struct module {
  module_type_e type;
  char *name;
  char **targets;
} module_t;

module_t *modules;
int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  int lines = 0;
  while (fgets(line, sizeof(line), input)) {
    lines++;
  }

  rewind(input);

  modules = (module_t *)malloc(sizeof(module_t) * lines);
  for (int i = 0; fgets(line, sizeof(line), input); i++) {
    printf("Line %d: %s", i, line);

    // set up the module type
    char *first = strtok(line, " ");
    if (strcmp(first, "broadcaster") == 0) {
      modules[i].type = BROADCAST;
    } else if (first[0] == '%') {
      modules[i].type = FLIPFLOP;
    } else if (first[0] == '&') {
      modules[i].type = CONJUNCTION;
    }

    // set up the module name
    if (modules[i].type != BROADCAST) {
      modules[i].name = (char *)malloc(sizeof(char) * strlen(first) - 1);
      strcpy(modules[i].name, first + 1);
    }

    // set up the module targets
    char *UNUSED_ARROW = strtok(NULL, " ");
    int target_count = 0;
    char *target[16];
    while ((target[target_count] = strtok(NULL, ", ")) != NULL) {
      target_count++;
    }
    printf("Target count: %d\n", target_count);
    modules[i].targets = (char **)malloc(sizeof(char *) * target_count);
    for (int j = 0; j < target_count; j++) {
      modules[i].targets[j] = (char *)malloc(sizeof(char) * strlen(target[j]));
      strcpy(modules[i].targets[j], target[j]);
      printf("Target %d: %s\n", j, modules[i].targets[j]);
    }
  }
}
