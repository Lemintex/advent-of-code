#include <stdbool.h>

typedef struct
{
    int directionX, directionY;
} direction_t;

typedef struct node_t node_t;
struct node_t
{
    int mapX, mapY;
    int coolingValue;
    int totalHeatLoss;
    direction_t direction;
    int numSameDirection;
};

bool IsSeenNode(node_t* queueOfSeenNodes, int queueOfSeenNodesSize, node_t* node);
void Heapify(node_t* arr, int n, int i);
void InsertToPQ(node_t* arr, int* n, node_t* key);
void DeleteFromPQ(node_t* tree, int* n, int i);