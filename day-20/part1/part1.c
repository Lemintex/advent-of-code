#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int get_module_index_by_name(char *name);
module_t *modules;
int module_total = 0;

int broadcaster_index = 0;

// QUEUE STUFF
typedef struct node {
  module_t *module;
  struct node *next;
} node_t;

typedef struct queue {
  node_t *head;
  node_t *tail;
} queue_t;

void enqueue(queue_t *queue, module_t *module) {
  node_t *new_node = malloc(sizeof(node_t));
  new_node->module = module;
  new_node->next = NULL;
  if (queue->head == NULL) {
    queue->head = new_node;
    queue->tail = new_node;
  } else {
    queue->tail->next = new_node;
    queue->tail = new_node;
  }
}

module_t *dequeue(queue_t *queue) {
  if (queue->head == NULL) {
    return NULL;
  }
  node_t *node = queue->head;
  module_t *module = node->module;
  queue->head = node->next;
  free(node);
  return module;
}

queue_t create_queue() {
  queue_t queue;
  queue.head = NULL;
  queue.tail = NULL;
  return queue;
}

queue_t queue;
int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  int lines = 0;
  while (fgets(line, sizeof(line), input)) {
    lines++;
  }
  module_total = lines;
  modules = malloc(sizeof(module_t) * lines);
  rewind(input);
  for (int i = 0; fgets(line, sizeof(line), input); i++) {
    if (line[0] == '%' || line[0] == '&') {
      for (int j = 0; j < strlen(line) - 1; j++) {
        line[j] = line[j + 1];
      }
    }
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
  press_button_full(1);
  printf("Pulses: %d %d\n", pulses[LOW], pulses[HIGH]);
  printf("Total pulses: %d\n", pulses[LOW] + pulses[HIGH]);
}

void press_button_full(int times) {
  queue = create_queue();
  for (int i = 0; i < times; i++) {
    enqueue(&queue, &modules[broadcaster_index]);
    press_button_once();
  }
}

void press_button_once() {
  while (queue.head != NULL) {
    module_t *module = dequeue(&queue);
    handle_signal_for_module(module, module->pulse);
  }
  printf("Pulses: %d %d\n", pulses[LOW], pulses[HIGH]);
}

void handle_signal_for_module(module_t *module, pulse_e pulse) {
  static int num = 1;
  if (module->type == FLIPFLOP) {
    if (pulse == LOW) {
      if (module->pulse == LOW) {
        module->pulse = HIGH;
      } else {
        module->pulse = LOW;
      }
      for (int i = 0; i < module->module_count; i++) {
        pulses[module->pulse]++;
        int index = get_module_index_by_name(module->modules[i]);
        module_t *m = &modules[index];
        enqueue(&queue, m);
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
      int index = get_module_index_by_name(module->modules[i]);
      module_t *m = &modules[index];
      enqueue(&queue, m);
    }
  } else if (module->type == BUTTON) {
    for (int i = 0; i < module->module_count; i++) {
      pulses[pulse]++;
      int index = get_module_index_by_name(module->modules[i]);
      module_t *m = &modules[index];
      enqueue(&queue, m);
    }
  } else if (module->type == RELAY) {
    for (int i = 0; i < module->module_count; i++) {
      pulses[pulse]++;
      int index = get_module_index_by_name(module->modules[i]);
      module_t *m = &modules[index];
      enqueue(&queue, m);
    }
  }
}

int get_module_index_by_name(char *name) {
  for (int i = 0; i < module_total; i++) {
    if (strcmp(modules[i].name, name) == 0) {
      return i;
    }
  }
  return 0;
}
