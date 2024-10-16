#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  char name[4];
  char l[4];
  char r[4];
} node_t;

typedef struct ghost {
  node_t *node;
  int seen_count;
  int cycle_length;
} ghost_t;

node_t nodes[750];
int node_count = 750;
ghost_t *ghosts;
int ghost_count = 6;

char instructions[278];

void get_lcm();
long long int gcd(long long int a, long long int b);

node_t *Find_node(char *name) {
  for (int i = 0; i < node_count; i++) {
    if (nodes[i].name[0] == name[0] && nodes[i].name[1] == name[1] &&
        nodes[i].name[2] == name[2]) {
      return &nodes[i];
    }
  }
  return NULL;
}

void get_ghost_cycle_length() {
  int total = 0;
  for (int g = 0; g < ghost_count; g++) {
    int path_index = 0;
    node_t *current = ghosts[g].node;
    int steps = 0;
    while (ghosts[g].seen_count < 2) {
      if (current->name[2] == 'Z') {
        if (ghosts[g].seen_count == 0) {
          ghosts[g].cycle_length = steps;
        } else {
          ghosts[g].cycle_length = steps - ghosts[g].cycle_length;
          ghosts[g].seen_count++;
          break;
        }
        ghosts[g].seen_count++;
      }
      if (instructions[path_index] == 'L') {
        current = Find_node(current->l);
      } else if (instructions[path_index] == 'R') {
        current = Find_node(current->r);
      }
      path_index++;
      if (path_index > 276)
        path_index = 0;
      steps++;
    }
  }
  for (int g = 0; g < ghost_count; g++) {
    printf("\n_ghost %d cycle: %d", g, ghosts[g].cycle_length);
  }
}

void parse_node(char *line, int index) {
  static int ghost_count = 0;
  nodes[index].name[0] = line[0];
  nodes[index].name[1] = line[1];
  nodes[index].name[2] = line[2];
  nodes[index].name[3] = '\0';

  nodes[index].l[0] = line[7];
  nodes[index].l[1] = line[8];
  nodes[index].l[2] = line[9];
  nodes[index].l[3] = '\0';

  nodes[index].r[0] = line[12];
  nodes[index].r[1] = line[13];
  nodes[index].r[2] = line[14];
  nodes[index].r[3] = '\0';

  if (line[2] == 'A') {
    ghosts[ghost_count].node = &nodes[index];
    ghosts[ghost_count].seen_count = 0;
    ghosts[ghost_count].cycle_length = 0;
    ghost_count++;
    printf("\n_ghost %d: %s", ghost_count, nodes[index].name);
  }
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

void get_lcm() {
  long long int ans = ghosts[0].cycle_length;
  for (int g = 1; g < ghost_count; g++) {
    ans =
        (((ghosts[g].cycle_length * ans)) / (gcd(ghosts[g].cycle_length, ans)));
  }
  printf("\n_gCD: %lld", ans);
}

long long int gcd(long long int a, long long int b) {
  if (b == 0) {
    return a;
  }
  return gcd(b, a % b);
}

int main() {
  ghosts = (ghost_t *)malloc(sizeof(ghost_t) * 6);
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
  get_ghost_cycle_length();
  get_lcm();
}
