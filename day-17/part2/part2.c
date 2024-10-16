// this needs entirely rewritten, it's a mess and takes 4 minutes to run

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct direction {
  int x;
  int y;
} direction_t;

typedef struct node {
  int x, y;
  int initial_heat_loss;
  direction_t facing;
  int steps_in_same_direction;
  int heat_loss;

  int prev_x, prev_y;
  bool is_part_of_path;
} node_t;

node_t pq[10000000];
int pq_count = 0;

node_t seen_nodes[10000000];
int seen_nodes_count = 0;

node_t *map;
int map_width = 0, map_height = 0;

void add_to_seen_nodes(node_t *node) { seen_nodes[seen_nodes_count++] = *node; }

bool is_in_seen_nodes(node_t node) {
  for (int i = 0; i < seen_nodes_count; i++) {
    if (seen_nodes[i].x == node.x && seen_nodes[i].y == node.y &&
        seen_nodes[i].facing.x == node.facing.x &&
        seen_nodes[i].facing.y == node.facing.y &&
        seen_nodes[i].steps_in_same_direction == node.steps_in_same_direction) {
      return true;
    }
  }
  return false;
}

void enqueue(node_t node) { pq[pq_count++] = node; }

node_t dequeue() {
  int lowest_heat_loss = 999999;
  int lowest_heat_loss_index = 0;
  for (int i = 0; i < pq_count; i++) {
    if (pq[i].heat_loss < lowest_heat_loss) {
      lowest_heat_loss = pq[i].heat_loss;
      lowest_heat_loss_index = i;
    }
  }
  node_t node = pq[lowest_heat_loss_index];
  for (int i = lowest_heat_loss_index; i < pq_count - 1; i++) {
    pq[i] = pq[i + 1];
  }
  pq_count--;
  return node;
}

int run_dijkstras() {
  int hits = 0;
  node_t start_node = map[0];
  start_node.heat_loss = 0;
  enqueue(start_node);
  while (pq_count > 0) {
    // get the node with the lowest total heat loss from the priority queue
    node_t node = dequeue();

    // if the node is already in the queue of seen nodes, skip it
    if (is_in_seen_nodes(node)) {
      continue;
    }

    // if not seen, add it to the queue of seen nodes so we don't process it
    // again
    add_to_seen_nodes(&node);

    // for each neighbor
    for (int i = 0; i < 4; i++) {
      node_t neighbor =
          (node_t){0, 0, 0, (direction_t){0, 0}, 0, 0, 0, 0, false};
      switch (i) {
      case 0:
        if (node.x > 0) {
          hits++;
          neighbor = map[(node.y * map_width) + (node.x - 1)];
          neighbor.facing = (direction_t){-1, 0};
        }
        break;
      case 1:
        if (node.x < map_width - 1) {
          neighbor = map[(node.y * map_width) + (node.x + 1)];
          neighbor.facing = (direction_t){1, 0};
        }
        break;
      case 2:
        if (node.y > 0) {
          neighbor = map[((node.y - 1) * map_width) + node.x];
          neighbor.facing = (direction_t){0, -1};
        }
        break;
      case 3:
        if (node.y < map_height - 1) {
          neighbor = map[((node.y + 1) * map_width) + node.x];
          neighbor.facing = (direction_t){0, 1};
        }
        break;
      }

      if ((neighbor.x == -1 && neighbor.y == -1) ||
          (neighbor.facing.x == -node.facing.x &&
           neighbor.facing.y == -node.facing.y) ||
          is_in_seen_nodes(neighbor)) {
        continue;
      }

      // if the neighbor is the same direction as the current node
      if (neighbor.facing.x == node.facing.x &&
              neighbor.facing.y == node.facing.y ||
          node.x == 0 && node.y == 0) {
        // increment the consecutive direction count if <3 and add the neighbor
        // to the priority queue with the total heat loss of the current node +
        // the heat loss of the neighbor
        if (node.steps_in_same_direction >= 10) {
          continue;
        }
        neighbor.steps_in_same_direction = node.steps_in_same_direction + 1;
      } else {
        if (node.steps_in_same_direction < 4) {
          continue;
        }
        neighbor.steps_in_same_direction = 1;
      }

      if (neighbor.x == map_width - 1 && neighbor.y == map_height - 1) {
        printf("Found the end node!\n");
        neighbor.heat_loss += node.heat_loss;
        printf("Total heat loss: %d\n", neighbor.heat_loss);
        return node.heat_loss;
      }

      neighbor.heat_loss += node.heat_loss;
      enqueue(neighbor);
    }
  }
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  while (fgets(line, sizeof(line), input)) {
    map_height++;
  }
  map_width = strlen(line);

  map = (node_t *)malloc(map_height * map_width * sizeof(node_t));
  rewind(input);
  for (int i = 0; i < map_height; i++) {
    fgets(line, sizeof(line), input);
    for (int j = 0; j < map_width; j++) {
      node_t *node = &map[i * map_width + j];
      node->x = j;
      node->y = i;
      node->heat_loss = line[j] - '0';
      node->initial_heat_loss = node->heat_loss;
      node->is_part_of_path = false;
    }
  }
  run_dijkstras();
}
