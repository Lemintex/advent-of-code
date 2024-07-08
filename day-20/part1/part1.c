#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pulses[2] = {0, 0};

typedef enum module_type { FLIPFLOP, CONJUNCTION, BROADCAST } module_type_e;
typedef enum pulse { LOW, HIGH } pulse_e;

typedef struct input_memory {
  char *name;
  pulse_e pulse;
} input_memory_t;

typedef struct module_memory {
  pulse_e pulse;
  input_memory_t *inputs;
} module_memory_t;

typedef struct module {
  module_type_e type;
  char *name;
  char **targets;
  int target_count;
  module_memory_t memory;
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
  int module_count = lines;
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

    char *rest = strtok(NULL, "\n");
    rest += 3;
    // set up the module targets
    char *target = strtok(rest, ",");
    while (target != NULL) {
      modules[i].targets = (char **)realloc(
          modules[i].targets, sizeof(char *) * (modules[i].target_count + 1));
      modules[i].targets[modules[i].target_count] =
          (char *)malloc(sizeof(char) * strlen(target));
      strcpy(modules[i].targets[modules[i].target_count], target);
      modules[i].target_count++;
      target = strtok(NULL, ",");
      printf("Target: %s\n", modules[i].targets[modules[i].target_count - 1]);
      printf(("Length: %d\n"),
             strlen(modules[i].targets[modules[i].target_count - 1]));
      if (target != NULL) {
        target += 1;
      }
    }
  }
  // debug print
  for (int i = 0; i < module_count; i++) {
    printf("Module %d: %s\n", i, modules[i].name);
    printf("Type: %d\n", modules[i].type);
    for (int j = 0; j < modules[i].target_count; j++) {
      printf("Target %d: %s\n", j, modules[i].targets[j]);
    }
  }
}
