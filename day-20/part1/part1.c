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

module_t *modules;
int module_count;

int broadcaster_index = 0;
int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  int lines = 0;
  while (fgets(line, sizeof(line), input)) {
    lines++;
  }
  modules = malloc(sizeof(module_t) * lines);
  rewind(input);
  for (int i = 0; fgets(line, sizeof(line), input); i++) {
    char *module_name = strtok(line, " ");
    if (strcmp(module_name, "broadcaster") == 0) {
      broadcaster_index = i;
      printf("Broadcaster index: %d\n", broadcaster_index);
    }
    char *unused = strtok(NULL, " ");

    char *tok[16];
    char *token = strtok(NULL, ", ");
    int n = 0;
    while (token != NULL) {
      tok[n++] = token;
      token = strtok(NULL, ", ");
    }
    modules[i].modules = malloc(sizeof(char *) * n);
    for (int j = 0; j < n; j++) {
      modules[i].modules[j] = malloc(sizeof(char) * 2);
      strcpy(modules[i].modules[j], tok[j]);
    }
    strcpy(modules[i].name, module_name);
    if (module_name[0] == '%') {
      modules[i].type = FLIPFLOP;
    } else if (module_name[0] == '&') {
      modules[i].type = CONJUNCTION;
    } else if (module_name[0] == 'b') {
      modules[i].type = BUTTON;
    } else {
      modules[i].type = RELAY;
    }
    printf("Module %s, type %d, modules: ", modules[i].name, modules[i].type);
    for (int j = 0; j < n; j++) {
      printf("%s ", modules[i].modules[j]);
    }
    printf("\n");
  }
}
