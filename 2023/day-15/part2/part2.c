#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *label;
  int focal_length;
} lens_t;

typedef struct {
  lens_t *lens;
  int count;
} box_t;

typedef struct {
  char *label;
  char *instruction; // can be - or 0-9
} instruction_t;

box_t hashmap[256]; // Holiday ASCII String Helper Manual Arrangement Procedure

int run_hash(char *l);

void add_lens(box_t *box, char *label, int focal_length) {
  for (int i = 0; i < box->count; i++) {
    if (strcmp(box->lens[i].label, label) == 0) {
      box->lens[i].focal_length = focal_length;
      return;
    }
  }
  box->count++;
  box->lens = realloc(box->lens, box->count * sizeof(lens_t));
  box->lens[box->count - 1].label = label;
  box->lens[box->count - 1].focal_length = focal_length;
}

void remove_lens(box_t *box, char *lens) {
  for (int i = 0; i < box->count; i++) {
    if (strcmp(box->lens[i].label, lens) == 0) {
      for (int j = i; j < box->count - 1; j++) {
        box->lens[j] = box->lens[j + 1];
      }
      box->count--;
      box->lens = realloc(box->lens, box->count * sizeof(lens_t));
      return;
    }
  }
}

void execute_instruction(instruction_t *instruction) {
  int label_hash = run_hash(instruction->label);

  if (strcmp(instruction->instruction, "-") == 0) {
    // remove lens
    remove_lens(&hashmap[label_hash], instruction->label);
  } else {
    // add lens
    add_lens(&hashmap[label_hash], instruction->label,
             atoi(instruction->instruction));
  }
}

int run_hash(char *l) {
  int hash = 0;
  for (int i = 0; i < strlen(l); i++) {
    hash += l[i];
    hash *= 17;
    hash %= 256;
  }
  return hash;
}

int get_part2result() {
  int result = 0;
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < hashmap[i].count; j++) {
      int n = (i + 1) * (j + 1) * hashmap[i].lens[j].focal_length;
      result += n;
    }
  }
  return result;
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[200000] = {0};

  int total = 0;
  while (fgets(line, sizeof(line), input)) {
    char *token = strtok(line, ",");

    char *label, *instruction, *focal_length;
    while (token != NULL) {
      label = token;
      instruction = strchr(token, '=');
      focal_length = strchr(token, '-');

      if (instruction != NULL) {
        *instruction = '\0'; // Separate label from instruction
        instruction++;
      } else if (focal_length != NULL) {
        *focal_length = '\0'; // Separate label from lens number
        focal_length++;
      }

      instruction_t inst = {0};
      inst.label = label;
      inst.instruction = instruction ? instruction : "-";
      execute_instruction(&inst);
      token = strtok(NULL, ",");
    }
  }
  printf("Part 2: %d\n", get_part2result());
}
