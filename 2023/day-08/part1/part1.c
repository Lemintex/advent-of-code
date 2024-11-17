#include <stdio.h>

typedef struct node {
  char name[3];
  char l[3];
  char r[3];
} node_t;

typedef struct ghost {
  int seen_count;
  int cycle_length;
} ghost_t;

node_t nodes[750];
int node_count = 750;

char instructions[278];

node_t* find_node(char *name) {
  for (int i = 0; i < node_count; i++) {
    if (nodes[i].name[0] == name[0] && nodes[i].name[1] == name[1] &&
        nodes[i].name[2] == name[2]) {
      return &nodes[i];
    }
  }
  return NULL;
}

void follow_path() {
  int total = 0;
  int path_index = 0;
  node_t* current = &nodes[0];
  while (current->name[0] != 'Z' || current->name[1] != 'Z' ||
         current->name[2] != 'Z') {
    if (instructions[path_index] == 'L') {
      current = find_node(current->l);
    } else if (instructions[path_index] == 'R') {
      current = find_node(current->r);
    }
    path_index++;
    if (path_index > 276) {
      path_index = 0;
    }
    total++;
  }
  printf("Total: %d\n", total);
}

void parse_node(char *line, int index) {
  static int ghost_count = 0;
  nodes[index].name[0] = line[0];
  if (line[0] == 'Z') {
    ghost_count++;
  }
  nodes[index].name[1] = line[1];
  nodes[index].name[2] = line[2];

  nodes[index].l[0] = line[7];
  nodes[index].l[1] = line[8];
  nodes[index].l[2] = line[9];

  nodes[index].r[0] = line[12];
  nodes[index].r[1] = line[13];
  nodes[index].r[2] = line[14];
}

void parse_instructions(char *line) {
  int i = 0;
  int j = 0;
  while (line[i] != '\0') {
    if (line[i] == ' ') {
      i++;
      continue;
    }
    instructions[j] = line[i];
    i++;
    j++;
  }
}

int main() {
  int node_index = 0;
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[512];
  int i = 0;
  while (fgets(line, sizeof(line), input)) {
    if (i == 0) {
      parse_instructions(line);
    } else if (i > 1) {
      parse_node(line, node_index);
      node_index++;
    }
    i++;
  }
  follow_path();
}
