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
  module_t **array;
} queue_t;

queue_t *create_queue(unsigned capacity) {
  queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
  queue->capacity = capacity;
  queue->front = queue->size = 0;
  queue->rear = capacity - 1;
  queue->array = (module_t **)malloc(queue->capacity * sizeof(module_t*));
  return queue;
}

int is_full(queue_t *queue) { return (queue->size == queue->capacity); }

int is_empty(queue_t *queue) { return (queue->size == 0); }

void enqueue(queue_t *queue, module_t* item) {
  if (is_full(queue)) {
    return;
  }
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = item;
  queue->size = queue->size + 1;
}

module_t* dequeue(queue_t *queue) {
  if (is_empty(queue)) {
    module_t* empty;
    return empty;
  }
  module_t* item = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  return item;
}
int broadcast_index = 0;
module_t *modules;
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
  int module_count = lines;
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
}

void do_button_presses() {
  int low = 0;
  int high = 0;
  for (int i = 0; i < 1000; i++)
  {
    low++;

    enqueue(queue, &(modules[broadcast_index]));
    /* code */
  }
  
}

void button_press() {

}

void send_pulse() {

}