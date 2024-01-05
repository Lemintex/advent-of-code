#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

int numberOfTilesInSameDirection = 0;
typedef enum
{
    NONE,
    NORTH,
    EAST,
    SOUTH,
    WEST
} direction_t;

typedef struct
{
    int mapX, mapY;
    int coolingValue;
    int pathValue;
    direction_t direction;
    int numSameDirection;
    char directionChar;
    bool visited;
} node_t;

node_t** visitedNodes;

node_t* tree;
node_t** map;
int mapWidth = 0, mapHeight = 0;

// PRIORITY QUEUE
// ---------------------------------------------
int size = 0;

void swap(node_t* a, node_t* b)
{
    node_t temp = *b;
    *b = *a;
    *a = temp;
}


// heapify the tree
void heapify(int array[], int size, int i)
{
    if (size == 1)
    {
        printf("Single element in the heap");
    }
    else
    {
        // Find the largest among root, left child and right child
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        if (l < size && array[l] > array[largest])
        {
            largest = l;
        }
        if (r < size && array[r] > array[largest])
        {
            largest = r;
        }
  
        // Swap and continue heapifying if root is not largest
        if (largest != i)
        {
          swap(&array[i], &array[largest]);
          heapify(array, size, largest);
        }
    }
}


// insert an element into the tree
void insert(node_t newNode)
{
    if (size == 0)
    {
        tree[0] = newNode;
        size += 1;
    }
    else
    {
        tree[size] = newNode;
        size += 1;
        for (int i = size / 2 - 1; i >= 0; i--)
        {
          heapify(size, i);
        }
    }
}

// delete an element from the tree
void deleteRoot(int num)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (num == tree[i].pathValue)
        {
            break;
        }
    }

    swap(&tree[i], &tree[size - 1]);
    size -= 1;
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heapify(size, i);
    }
}

// ---------------------------------------------
// END PRIORITY QUEUE

void Dijkstra()
{
    visitedNodes = (node_t**)malloc(mapHeight * mapWidth * sizeof(node_t*));

    // start at the top left
    node_t* currentNode = &map[0][0];
    currentNode->pathValue = 0;
    currentNode->visited = true;
    visitedNodes[0] = currentNode;
    int x = 0, y = 0;

    // while we haven't reached the bottom right

    while (currentNode->mapX != map[mapHeight - 1][mapWidth - 1].mapX || currentNode->mapY != map[mapHeight - 1][mapWidth - 1].mapY)
    {
        // check the 4 adjacent nodes
        if (currentNode->mapY > 0 && ((currentNode->numSameDirection >= 3 && currentNode->direction == map[currentNode->mapY - 1][currentNode->mapX].direction) || currentNode->direction == NONE))
        {
            node_t* northNode = &map[currentNode->mapY - 1][currentNode->mapX];
            if (!northNode->visited)
            {
                northNode->pathValue = currentNode->pathValue + northNode->coolingValue;
                northNode->direction = NORTH;
                northNode->numSameDirection = currentNode->numSameDirection + 1;

                // if they are not visited, add them to the priority queue
                insert(*northNode);
            }
        }

        if (currentNode->mapX < mapWidth - 1 && ((currentNode->numSameDirection >= 3 && currentNode->direction == map[currentNode->mapY][currentNode->mapX + 1].direction) || currentNode->direction == NONE))
        {
            node_t* eastNode = &map[currentNode->mapY][currentNode->mapX + 1];
            if (!eastNode->visited)
            {
                eastNode->pathValue = currentNode->pathValue + eastNode->coolingValue;
                eastNode->direction = EAST;
                eastNode->numSameDirection = currentNode->numSameDirection + 1;

                // if they are not visited, add them to the priority queue
                insert(*eastNode);
            }
        }

        if (currentNode->mapY < mapHeight - 1 && ((currentNode->numSameDirection >= 3 && currentNode->direction == map[currentNode->mapY + 1][currentNode->mapX].direction) || currentNode->direction == NONE))
        {
            node_t* southNode = &map[currentNode->mapY + 1][currentNode->mapX];
            if (!southNode->visited)
            {
                southNode->pathValue = currentNode->pathValue + southNode->coolingValue;
                southNode->direction = SOUTH;
                southNode->numSameDirection = currentNode->numSameDirection + 1;

                // if they are not visited, add them to the priority queue
                insert(*southNode);
            }
        }

        if (currentNode->mapX > 0 && ((currentNode->numSameDirection >= 3 && currentNode->direction == map[currentNode->mapY][currentNode->mapX - 1].direction) || currentNode->direction == NONE))
        {
            node_t* westNode = &map[currentNode->mapY][currentNode->mapX - 1];
            if (!westNode->visited)
            {
                westNode->pathValue = currentNode->pathValue + westNode->coolingValue;
                westNode->direction = WEST;
                westNode->numSameDirection = currentNode->numSameDirection + 1;
                // if they are not visited, add them to the priority queue
                insert(*westNode);
            }
        }
                // if they are visited, check if the path value is lower `than the current path value
                // if it is, update the path value and direction
                // if it isn't, do nothing
                // then, remove the current node from the priority queue
                // set the current node to the next node in the priority queue
                // repeat
                // if the priority queue is empty, we are done
                // if the current node is the bottom right, we are done

    }
}
int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    while (fgets(line, sizeof(line), input))
    {
        mapHeight++;
    }
    mapWidth = strlen(line) - 1;
    map = (node_t**)malloc(mapHeight * sizeof(node_t*));
    rewind(input);
    for (int i = 0; i < mapHeight; i++)
    {
        fgets(line, sizeof(line), input);
        map[i] = (node_t*)malloc(mapWidth * sizeof(node_t));
        for (int j = 0; j < mapWidth; j++)
        {
            node_t* node = &map[i][j];
            node->mapX = j;
            node->mapY = i;
            node->coolingValue = line[j] - '0';
            node->pathValue = 99999999;
            node->visited = false;
            node->direction = NONE;
        }
    }

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            printf("%d", map[i][j].coolingValue);
        }
        printf("\n");
    }
    
}