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

typedef struct queue_item {
  module_t *module;
  module_t *target;
  pulse_e pulse;
} queue_item_t;

typedef struct queue {
  int front, rear, size;
  unsigned capacity;
  queue_item_t *array;
} queue_t;

void do_button_presses();
void button_press(long int *low, long int *high);
module_t *is_in_module(module_t *mod);
module_t *is_in_module_str(char *str);
void send_pulse_to_targets(module_t *mod, pulse_e pulse);
void update_targets_memory_flipflop(module_t *mod);
void update_targets_memory_conjunction(module_t *mod);

queue_t *create_queue(unsigned capacity) {
  queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
  queue->capacity = capacity;
  queue->front = queue->size = 0;
  queue->rear = capacity - 1;
  queue->array = (queue_item_t *)malloc(queue->capacity * sizeof(queue_item_t));
  return queue;
}

int is_full(queue_t *queue) { return (queue->size == queue->capacity); }

int is_empty(queue_t *queue) { return (queue->size == 0); }

void enqueue(queue_t *queue, queue_item_t item) {
  if (is_full(queue)) {
    return;
  }
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = item;
  queue->size = queue->size + 1;
  // printf("\nenqued %s\n", item.target->name);
}

queue_item_t dequeue(queue_t *queue) {
  if (is_empty(queue)) {
    return (queue_item_t){};
  }
  queue_item_t item = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  return item;
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
    if (i == 54)
      continue;
    // set up the module targets
    printf("Mod %d: \n", i);
    char *target = strtok(rest[i], ",\n");
    while (target != NULL) {
      // printf("%s len %d, ", target, strlen(target));
      for (int l = 0; l < strlen(target); l++) {
        printf("%d ", target[l]);
      }
      modules[i].target.targets = (module_t **)realloc(
          modules[i].target.targets,
          sizeof(module_t *) * modules[i].target.target_count + 1);
      module_t *t = is_in_module_str(target);
      modules[i].target.targets[modules[i].target.target_count] = t;
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
  // set up the module inputs
  for (int i = 0; i < module_count; i++) {
    printf("%d\n", i);
    modules[i].input.inputs = (module_t **)malloc(0);
    for (int j = 0; j < modules[i].target.target_count; j++) {
      for (int k = 0; k < module_count; k++) {
        if (i == k || modules[k].type == BROADCAST || i == 54) {
          continue;
        }
        if (strcmp(modules[i].target.targets[j]->name, modules[k].name) == 0) {
          // allocate memory for the new input
          modules[i].input.inputs = (module_t **)realloc(
              modules[i].input.inputs,
              sizeof(module_t *) * (modules[i].input.input_count + 1));
          modules[i].input.inputs[modules[i].input.input_count] = &modules[k];

          // increment the input count
          modules[i].input.input_count++;
          break;
        }
      }
    }
  }
  for (int i = 0; i < module_count; i++) {
    printf("Module %d: %s\n", i, modules[i].name);
    printf("Input count: %d\n", modules[i].input.input_count);
    for (int j = 0; j < modules[i].input.input_count; j++) {
      printf("Input %d: %s\n", j, modules[i].input.inputs[j]->name);
      printf("Pulse: %d\n", modules[i].input.inputs[j]->input.pulse);
    }
    for (int j = 0; j < modules[i].target.target_count; j++) {
      printf("Target %d: %s\n", j, modules[i].target.targets[j]->name);
    }
  }
  do_button_presses();
}

void do_button_presses() {
  long int low = 0;
  long int high = 0;
  for (int i = 0; i < 1000; i++) {
    low++;
    button_press(&low, &high);
  }
  printf("%d\n", low);
  printf("%d\n", high);
  printf("%ld\n", high * low);
}

void button_press(long int *low, long int *high) {
  for (int n = 0; n < modules[broadcast_index].target.target_count; n++) {
    module_t *broadcast = &modules[broadcast_index];
    module_t *target = broadcast->target.targets[n];
    queue_item_t item = (queue_item_t){broadcast, target, LOW};
    enqueue(queue, item);
  }
  // int num = 0;
  // int max = 10;
  while (!is_empty(queue)) {
    queue_item_t item = dequeue(queue);
    // printf("Origin: %s\nTarget: %s\n Pulse: %d\n----------------------------\n", item.module->name, item.target->name, item.pulse);
    // if (num++ == max) return;
    module_t *origin = item.module;
    module_t *target = item.target;
    pulse_e pulse = item.pulse;
    // printf("\nName: %s\nType:%d\nPulse:%d\n", target->name,
    //        target->type, target->input.pulse);
    // for (int i = 0; i < target->target.target_count; i++) {
    //   printf("\nTarget %d: %s", i, target->target.targets[i]->name);
    // }
    if (pulse == LOW) {
      *low = *low + 1;
    } else {
      *high = *high + 1;
    }
    pulse_e output;

    switch (target->type) {
    case FLIPFLOP:
      if (pulse == HIGH) {
        break;
      }
      // if this guard is passed we flip the output
      if (target->input.pulse == LOW) {
        target->input.pulse = HIGH;
      } else {
        target->input.pulse = LOW;
      }
      output = target->input.pulse;
        // printf("\nTarget count: %d\n", target->target.target_count);
      // WRONG
      for (int i = 0; i < target->target.target_count; i++) {
          module_t *t = is_in_module_str(target->target.targets[i]->name);
        queue_item_t item_new = {target, t, output};
    // printf("\nENQUE\nOrigin: %s\nTarget: %s\n Pulse: %d\n----------------------------\n", item_new.module->name, item_new.target->name, item_new.pulse);
        enqueue(queue, item_new);
      }

      break;

    case CONJUNCTION:
      output = LOW;
      for (int i = 0; target->input.input_count; i++) {
        if (target->input.inputs[i]->input.pulse == LOW) {
          output = HIGH;
          break;
        }
      }

      origin->input.pulse = output;
        // printf("\nTarget count: %d\n", target->target.target_count);
      for (int i = 0; i < target->target.target_count; i++) {
          module_t *t = is_in_module_str(target->target.targets[i]->name);
        queue_item_t item_new = (queue_item_t){target, t, output};
        enqueue(queue, item_new);
      }

      break;
    }
  }
}

module_t *is_in_module(module_t *mod) {
  for (int i = 0; i < module_count; i++) {
    if (strcmp(modules[i].name, mod->name) == 0) {
      return &modules[i];
    }
  }
  return NULL;
}

module_t *is_in_module_str(char *str) {
  for (int i = 0; i < module_count; i++) {
    if (strcmp(modules[i].name, str) == 0) {
      return &modules[i];
    }
  }
  return NULL;
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
// 3268229565 too high
