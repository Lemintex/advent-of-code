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
  int input_count;
} module_memory_t;

typedef struct module {
  module_type_e type;
  char *name;
  char **targets;
  int target_count;
  module_memory_t memory;
} module_t;

typedef struct queue {
  int front, rear, size;
  unsigned capacity;
  int *array;
} queue_t;

void do_button_presses();
void button_press(int *low, int *high);
int is_in_module(char *mod);

queue_t *create_queue(unsigned capacity) {
  queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
  queue->capacity = capacity;
  queue->front = queue->size = 0;
  queue->rear = capacity - 1;
  queue->array = (int *)malloc(queue->capacity * sizeof(int));
  return queue;
}

int is_full(queue_t *queue) { return (queue->size == queue->capacity); }

int is_empty(queue_t *queue) { return (queue->size == 0); }

void enqueue(queue_t *queue, int itemIndex) {
  if (is_full(queue)) {
    return;
  }
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = itemIndex;
  queue->size = queue->size + 1;
}

int dequeue(queue_t *queue) {
  if (is_empty(queue)) {
    return -1;
  }
  int item = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  return item;
}
int broadcast_index = 0;
module_t *modules;
int module_count;
queue_t *queue;

int main() {
  queue = create_queue(100);
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  int lines = 0;
  while (fgets(line, sizeof(line), input)) {
    lines++;
  }

  rewind(input);

  modules = (module_t *)malloc(sizeof(module_t) * lines);
  module_count = lines;
  for (int i = 0; fgets(line, sizeof(line), input); i++) {
    modules[i].memory.input_count = 0;
    modules[i].memory.inputs = NULL;
    modules[i].memory.pulse = LOW;
    printf("Line %d: %s", i, line);

    // set up the module type
    char *first = strtok(line, " ");
    if (strcmp(first, "broadcaster") == 0) {
      broadcast_index = i;
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
    } else {
      modules[i].name = (char *)malloc(sizeof(char) * strlen(first));
      strcpy(modules[i].name, first);
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
    for (int j = 0; j < modules[i].target_count; j++) {
      printf("Target %d: %s\n", j, modules[i].targets[j]);
    }
  }
  // set up the module targets
  for (int i = 0; i < module_count; i++) {
    modules[i].memory.inputs =
        (input_memory_t *)malloc(sizeof(input_memory_t) * 0);
    for (int j = 0; j < modules[i].target_count; j++) {
      for (int k = 0; k < module_count; k++) {
        if (i == k || modules[k].type == BROADCAST) {
          continue;
        }
        if (strcmp(modules[i].targets[j], modules[k].name) == 0) {
          // allocate memory for the new input
          modules[i].memory.inputs = (input_memory_t *)realloc(
              modules[i].memory.inputs,
              sizeof(input_memory_t) * (modules[i].memory.input_count + 1));

          // allocate memory for the name of the input
          modules[i].memory.inputs[modules[i].memory.input_count].name =
              (char *)malloc(sizeof(char) * strlen(modules[k].name));

          // copy the name of the input
          strcpy(modules[i].memory.inputs[modules[i].memory.input_count].name,
                 modules[k].name);

          // set the pulse to low
          modules[i].memory.inputs[modules[i].memory.input_count].pulse = LOW;

          // increment the input count
          modules[i].memory.input_count++;
          break;
        }
      }
    }
  }
  for (int i = 0; i < module_count; i++) {
    printf("Module %d: %s\n", i, modules[i].name);
    printf("Input count: %d\n", modules[i].memory.input_count);
    for (int j = 0; j < modules[i].memory.input_count; j++) {
      printf("Input %d: %s\n", j, modules[i].memory.inputs[j].name);
      printf("Pulse: %d\n", modules[i].memory.inputs[j].pulse);
    }
    for (int j = 0; j < modules[i].target_count; j++) {
      printf("Target %d: %s\n", j, modules[i].targets[j]);
    }
  }
  do_button_presses();
}

void do_button_presses() {
  int low = 0;
  int high = 0;
  for (int i = 0; i < 1000; i++) {
    low++;
    enqueue(queue, broadcast_index);
    button_press(&low, &high);
  }
  printf("%d\n", low);
  printf("%d\n", high);
  printf("%d\n", high * low);
}

void button_press(int *low, int *high) {
  while (!is_empty(queue)) {
    int i = dequeue(queue);
    module_t module = modules[i];
    if (module.memory.pulse == LOW) {
      *low = *low + 1;
    } else {
      *high = *high + 1;
    }
    pulse_e output;
    if (module.type == FLIPFLOP) {
      if (module.memory.pulse == LOW) {
        if (module.memory.pulse == LOW) {
          module.memory.pulse = HIGH;
        } else {
          module.memory.pulse = LOW;
        }
      }
      output = module.memory.pulse;

      for (int i = 0; i < module.target_count; i++) {
        modules[i].memory.pulse = module.memory.pulse;
        enqueue(queue, i);
      }
    } else if (module.type == CONJUNCTION) {
      output = HIGH;
      for (int i = 0; module.memory.input_count; i++) {
        if (module.memory.inputs[i].pulse == LOW) {
          output = LOW;
          break;
        }
      }
      for (int i = 0; i < module.target_count; i++) {
        int t = is_in_module(module.targets[i]);
        modules[t].memory.pulse = output;
      }
    }
  }
}

int is_in_module(char *mod) {
  for (int i = 0; i < module_count; i++) {
    if (strcmp(modules[i].name, mod)) {
      return i;
    }
  }
  return 0;
}
