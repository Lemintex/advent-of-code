#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/queue.h>

int pulses[2] = {0, 0};

typedef enum module_type { BUTTON, RELAY, FLIPFLOP, CONJUNCTION } module_type_e;
typedef enum pulse { LOW, HIGH } pulse_e;

typedef struct module {
  pulse_e pulse;
  char **modules;
  int module_count;
  char type;
  char name[2];
  int all_high;
} module_t;

void press_button_full(int times);
void press_button_once();
void handle_signal_for_module(module_t *module, pulse_e pulse);
module_t *get_module_by_name(char *name);
module_t *modules;
int module_total = 0;

int broadcaster_index = 0;

// QUEUE STUFF
SLIST_HEAD(slisthead, entry) head = SLIST_HEAD_INITIALIZER(head);
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
    modules[i].module_count = n;
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
  }
  press_button_full(1000);
  printf("Pulses: %d %d\n", pulses[LOW], pulses[HIGH]);
  printf("Total pulses: %d\n", pulses[LOW] + pulses[HIGH]);
}

void press_button_full(int times) {
  for (int i = 0; i < times; i++) {
    press_button_once();
  }
}

void press_button_once() {
  handle_signal_for_module(&modules[broadcaster_index], LOW);
}

void handle_signal_for_module(module_t *module, pulse_e pulse) {
  static int num = 1;
  printf("Num: %d\n", num++);
  printf("Module: %s\n", module->name);
  if (module->type == FLIPFLOP) {
    if (pulse == LOW) {
      if (module->pulse == LOW) {
        module->pulse = HIGH;
      } else {
        module->pulse = LOW;
      }
      for (int i = 0; i < module->module_count; i++) {
        pulses[module->pulse]++;
        module_t *m = get_module_by_name(module->modules[i]);
        handle_signal_for_module(m, module->pulse);
      }
    }
  } else if (module->type == CONJUNCTION) {
    if (pulse == LOW) {
      module->all_high = 0;
    }
    if (module->all_high) {
      pulse = HIGH;
    } else {
      pulse = LOW;
    }
    for (int i = 0; i < module->module_count; i++) {
      pulses[pulse]++;
      module_t *m = get_module_by_name(module->modules[i]);
      handle_signal_for_module(m, pulse);
    }
  } else if (module->type == BUTTON) {
    printf("Button %s\n", module->name);
    for (int i = 0; i < module->module_count; i++) {
      pulses[pulse]++;
      module_t *m = get_module_by_name(module->modules[i]);
      handle_signal_for_module(m, pulse);
    }
  }
}

module_t *get_module_by_name(char *name) {
  for (int i = 0; i < module_total; i++) {
    if (strcmp(modules[i].name, name) == 0) {
      return &modules[i];
    }
  }
  return NULL;
}