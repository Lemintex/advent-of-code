#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum module_type { FLIPFLOP, CONJUNCTION, BROADCAST } module_type_e;
typedef enum pulse { LOW, HIGH } pulse_e;

typedef struct module module_t;
typedef struct module_cycle module_cycle_t;
typedef struct module_memory {
  char **input_name;
  pulse_e *pulse;
  int input_count;
} module_memory_t;

typedef struct module_targets {
  char **target_name;
  pulse_e *pulse;
  int target_count;
} module_targets_t;

typedef struct module_cycle {
  int cycle_len; // first pass - set to presses ||| second pass - set to presses - cycle_len
  int seen_count;
} module_cycle_t;

typedef struct module {
  module_type_e type;
  char *name;
  pulse_e pulse;
  module_memory_t input;
  module_targets_t target;
  module_cycle_t cycle;
} module_t;

typedef struct queue_item {
  char *module_name;
  char *target_name;
  pulse_e pulse;
} queue_item_t;

typedef struct queue {
  int front, rear, size;
  unsigned capacity;
  queue_item_t *array;
} queue_t;

void do_button_presses();
void button_press();
module_t *is_in_module(module_t *mod);
module_t *is_in_module_str(char *str);
void send_pulse_to_targets(module_t *mod, pulse_e pulse);
void update_targets_memory_flipflop(module_t *mod);
void update_targets_memory_conjunction(module_t *mod);
int are_all_cycles_finished();

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
    modules[i].cycle.cycle_len = 0;
    modules[i].cycle.seen_count = 0;

    modules[i].input.input_count = 0;
    modules[i].target.target_count = 0;

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
    char *target = strtok(rest[i], ",\n");
    for (int str = 0; target != NULL; str++) {
      modules[i].target.target_name =
          (char **)realloc(modules[i].target.target_name,
                           sizeof(char *) * modules[i].target.target_count + 1);
      modules[i].target.target_name[str] = strdup(target);
      modules[i].target.target_count++;
      target = strtok(NULL, "\n,");
      if (target != NULL) {
        target += 1;
      }
    }
  }

  // set up the module inputs
  for (int i = 0; i < module_count; i++) {
    modules[i].input.input_name = (char **)malloc(0);
    for (int j = 0; j < module_count; j++) {
      for (int k = 0; k < modules[j].target.target_count; k++) {
        if (strcmp(modules[j].target.target_name[k], modules[i].name) == 0) {
          modules[i].input.input_name = (char **)realloc(
              modules[i].input.input_name,
              sizeof(char *) * modules[i].input.input_count + 1);
          modules[i].input.input_name[modules[i].input.input_count] =
              strdup(modules[j].name);
          modules[i].input.input_count++;
        }
      }
    }
  }
  do_button_presses();
}

// assuming that for all module states, there is a fixes number of button presses to reach that state again
void do_button_presses() {
  module_t* rx = is_in_module_str("rx");
  module_t* input = is_in_module_str(rx->input.input_name[0]);
  module_t* in_modules[input->input.input_count];
  for (int i = 0; i < input->input.input_count; i++) {
    in_modules[i] = is_in_module_str(input->input.input_name[i]);
  }
  int in_counts[4] = {0};
  while (1) {
    button_press(in_modules, in_counts, 4);
    int br = are_all_cycles_finished();
    if (br == 1) {
      long int total = 1;
      for (int c = 0; c < 4; c++) {
        total *= in_modules[c]->cycle.cycle_len;
      }
      printf("\nTotal: %ld", total);
    }
  }
}

void button_press(module_t* in_modules[], int* indexes, int len) {
  static int presses = 0;
  presses++;
  for (int n = 0; n < modules[broadcast_index].target.target_count; n++) {
    module_t *broadcast = &modules[broadcast_index];
    char *target = broadcast->target.target_name[n];
    queue_item_t item = (queue_item_t){broadcast->name, target, LOW};
    enqueue(queue, item);
  }
  while (!is_empty(queue)) {
    queue_item_t item = dequeue(queue);
    module_t *origin = is_in_module_str(item.module_name);
    module_t *target = is_in_module_str(item.target_name);
    pulse_e pulse = item.pulse;
    pulse_e output;

    // does the current module have a cycle length
    if (origin->cycle.seen_count < 2 && pulse == HIGH) {
      if (origin->cycle.seen_count == 0) {
        origin->cycle.cycle_len = presses;
      }
      else {
        origin->cycle.cycle_len = presses - origin->cycle.cycle_len;
      }
      origin->cycle.seen_count++;
    }

    switch (target->type) {
    case FLIPFLOP:
      if (pulse == HIGH) {
        break;
      }
      // if this guard is passed we flip the output
      if (target->pulse == LOW) {
        target->pulse = HIGH;
      } else {
        target->pulse = LOW;
      }
      output = target->pulse;
      for (int i = 0; i < target->target.target_count; i++) {
        module_t *t = is_in_module_str(target->target.target_name[i]);
        queue_item_t item_new = {target->name, t->name, output};
        enqueue(queue, item_new);
      }

      break;

    case CONJUNCTION:
      for (int i = 0; i < target->input.input_count; i++) {
        if (strcmp(origin->name, target->input.input_name[i]) == 0) {
          is_in_module_str(target->input.input_name[i])->pulse = pulse;
          break;
        }
      }
      output = LOW;
      for (int i = 0; i < target->input.input_count; i++) {
        if (is_in_module_str(target->input.input_name[i])->pulse == LOW) {
          output = HIGH;
          break;
        }
      }

      for (int i = 0; i < target->target.target_count; i++) {
        module_t *t = is_in_module_str(target->target.target_name[i]);
        queue_item_t item_new = (queue_item_t){target->name, t->name, output};
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

int are_all_cycles_finished() {
  for (int i = 0; i < module_count; i++) {
    if (modules[i].cycle.seen_count < 2 && (strcmp(modules[i].name, "rx") != 0 && strcmp(modules[i].name, "broadcaster") != 0)) {
      printf("\nModule %s not cycled", modules[i].name);
      return 0;
    }
  }
  return 1;
}