#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum module_type { FLIPFLOP, CONJUNCTION, BROADCAST } module_type_e;
typedef enum pulse { LOW, HIGH } pulse_e;

typedef struct module module_t;
typedef struct module_memory {
  pulse_e pulse;
  module_t **inputs;
  int input_count;
} module_memory_t;

typedef struct module_targets {
  module_t **targets;
  int target_count;
} module_targets_t;

typedef struct module {
  module_type_e type;
  char *name;
  module_memory_t input;
  module_targets_t target;
} module_t;

typedef struct queue {
  int front, rear, size;
  unsigned capacity;
  module_t **array;
} queue_t;

void do_button_presses();
void button_press(int *low, int *high);
int is_in_module(char *mod);
void send_pulse_to_targets(module_t *mod, pulse_e pulse);
void update_targets_memory_flipflop(module_t *mod);
void update_targets_memory_conjunction(module_t *mod);

queue_t *create_queue(unsigned capacity) {
  queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
  queue->capacity = capacity;
  queue->front = queue->size = 0;
  queue->rear = capacity - 1;
  queue->array = (module_t **)malloc(queue->capacity * sizeof(module_t *));
  return queue;
}

int is_full(queue_t *queue) { return (queue->size == queue->capacity); }

int is_empty(queue_t *queue) { return (queue->size == 0); }

void enqueue(queue_t *queue, module_t *module_ptr) {
  if (is_full(queue)) {
    return;
  }
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = module_ptr;
  queue->size = queue->size + 1;
}

module_t *dequeue(queue_t *queue) {
  if (is_empty(queue)) {
    return NULL;
  }
  module_t *mod = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  return mod;
}
int broadcast_index = 0;
module_t *modules;
int module_count;
queue_t *queue;

int main() {
  queue = create_queue(100000);
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  int lines = 0;
  while (fgets(line, sizeof(line), input)) {
    lines++;
  }

  rewind(input);

  char *rest[lines];
  modules = (module_t *)malloc(sizeof(module_t) * lines);
  module_count = lines;
  for (int i = 0; fgets(line, sizeof(line), input); i++) {
    modules[i].input.input_count = 0;
    modules[i].input.inputs = NULL;
    modules[i].input.pulse = LOW;
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

    rest[i] = strtok(NULL, "\n");
    rest[i] += 3;
  }
  rewind(input);
  for (int i = 0; fgets(line, sizeof(line), input); i++) {
    // set up the module targets
    printf("Mod %d: \n", i);
    char *target = strtok(rest[i], ",\n");
    while (target != NULL) {
      printf("%s len %d, ", target, strlen(target));
      for (int l = 0; l < strlen(target); l++) {
        printf("%d ", target[l]);
      }
      modules[i].target.targets = (module_t **)realloc(
          modules[i].target.targets,
          sizeof(module_t *) * modules[i].target.target_count + 1);
      int target_index = is_in_module(target);
      printf("%d, ", target_index);
      modules[i].target.targets[modules[i].target.target_count] =
          &modules[target_index];
      modules[i].target.target_count++;
      target = strtok(NULL, "\n,");
      if (target != NULL) {
        target += 1;
      }
    }
  }

  // debug print
  for (int i = 0; i < module_count; i++) {
    printf("Module %d: %s\n", i, modules[i].name);
    for (int j = 0; j < modules[i].target.target_count; j++) {
      printf("Target %d: %s\n", j, modules[i].target.targets[j]->name);
    }
  }
  // // set up the module targets
  // for (int i = 0; i < module_count; i++) {
  //   modules[i].memory.inputs =
  //       (input_memory_t *)malloc(sizeof(input_memory_t) * 0);
  //   for (int j = 0; j < modules[i].target_count; j++) {
  //     for (int k = 0; k < module_count; k++) {
  //       if (i == k || modules[k].type == BROADCAST) {
  //         continue;
  //       }
  //       if (strcmp(modules[i].targets[j], modules[k].name) == 0) {
  //         // allocate memory for the new input
  //         modules[i].memory.inputs = (input_memory_t *)realloc(
  //             modules[i].memory.inputs,
  //             sizeof(input_memory_t) * (modules[i].memory.input_count + 1));
  //
  //         // allocate memory for the name of the input
  //         modules[i].memory.inputs[modules[i].memory.input_count].name =
  //             (char *)malloc(sizeof(char) * strlen(modules[k].name));
  //
  //         // copy the name of the input
  //         strcpy(modules[i].memory.inputs[modules[i].memory.input_count].name,
  //                modules[k].name);
  //
  //         // set the pulse to low
  //         modules[i].memory.inputs[modules[i].memory.input_count].pulse =
  //         LOW;
  //
  //         // increment the input count
  //         modules[i].memory.input_count++;
  //         break;
  //       }
  //     }
  //   }
  // }
  // for (int i = 0; i < module_count; i++) {
  //   printf("Module %d: %s\n", i, modules[i].name);
  //   printf("Input count: %d\n", modules[i].memory.input_count);
  //   for (int j = 0; j < modules[i].memory.input_count; j++) {
  //     printf("Input %d: %s\n", j, modules[i].memory.inputs[j].name);
  //     printf("Pulse: %d\n", modules[i].memory.inputs[j].pulse);
  //   }
  //   for (int j = 0; j < modules[i].target_count; j++) {
  //     printf("Target %d: %s\n", j, modules[i].targets[j]);
  //   }
  // }
  // do_button_presses();
}
//
// void do_button_presses() {
//   int low = 0;
//   int high = 0;
//   for (int i = 0; i < 1; i++) {
//     low++;
//     button_press(&low, &high);
//   }
//   printf("%d\n", low);
//   printf("%d\n", high);
//   printf("%d\n", high * low);
// }
//
// void button_press(int *low, int *high) {
//   for (int n = 0; n < modules[broadcast_index].target_count; n++) {
//     int m = is_in_module(modules[broadcast_index].targets[n]);
//     if (m < 0) {
//       continue;
//     }
//     enqueue(queue, m);
//   }
//   while (!is_empty(queue)) {
//     int i = dequeue(queue);
//     module_t module = modules[i];
//     if (module.memory.pulse == LOW) {
//       *low = *low + 1;
//     } else {
//       *high = *high + 1;
//     }
//     pulse_e output;
//
//     switch (module.type) {
//     case FLIPFLOP:
//       if (module.pulse == HIGH) {
//         break;
//       }
//       // if this guard is passed we flip the output
//       if (module.memory.pulse == LOW) {
//         module.memory.pulse = HIGH;
//       } else {
//         module.memory.pulse = LOW;
//       }
//       output = module.memory.pulse;
//
//       // WRONG
//       for (int i = 0; i < module.target_count; i++) {
//         modules[i].memory.pulse = module.memory.pulse;
//         modules[i].pulse = module.memory.pulse;
//         enqueue(queue, i);
//       }
//
//       break;
//
//     case CONJUNCTION:
//       output = HIGH;
//       for (int i = 0; module.memory.input_count; i++) {
//         if (module.memory.inputs[i].pulse == LOW) {
//           output = LOW;
//           break;
//         }
//       }
//
//       module.pulse = output;
//       update_targets_memory_conjunction(&module);
//
//       break;
//     }
//   }
// }
//
int is_in_module(char *mod) {
  for (int i = 0; i < module_count; i++) {
    if (strcmp(modules[i].name, mod) == 0) {
      return i;
    }
  }
  return -1;
}
//
// void update_targets_memory_flipflop(module_t *mod) {
//   for (int i = 0; i < mod->target_count; i++) {
//     int target_index = is_in_module(mod->targets[i]);
//     if (target_index < 0) {
//       printf("ERROR THIS SHOULD NOT BE HAPPENING");
//       return;
//     }
//     module_t target = modules[target_index];
//   }
// }
//
// // for every target in mod update the targets memory for mod
// void update_targets_memory_conjunction(module_t *mod) {
//   for (int i = 0; i < mod->target_count; i++) {
//     int target_index = is_in_module(mod->targets[i]);
//     if (target_index < 0) {
//       printf("ERROR THIS SHOULD NOT BE HAPPENING");
//       return;
//     }
//     module_t target = modules[target_index];
//     switch (target.type) {
//     case FLIPFLOP:
//       // do something
//       break;
//
//     case CONJUNCTION:
//       for (int j = 0; j < target.memory.input_count; j++) {
//         if (strcmp(mod->name, target.memory.inputs[j].name) == 0) {
//           target.memory.inputs[j].pulse = mod->pulse;
//         }
//       }
//     }
//   }
// }
//
// void send_pulse_to_targets(module_t *mod, pulse_e pulse) {
//   for (int t = 0; t < mod->target_count; t++) {
//   }
// }
