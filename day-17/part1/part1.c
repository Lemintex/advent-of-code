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
    int coolingValue;
    int pathValue;
    direction_t direction;
    char directionChar;
    bool visited;
} node_t;

node_t** visitedNodes;

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
void insert(int array[], int newNum)
{
    if (size == 0)
    {
        array[0] = newNum;
        size += 1;
    }
    else
    {
        array[size] = newNum;
        size += 1;
        for (int i = size / 2 - 1; i >= 0; i--)
        {
          heapify(array, size, i);
        }
    }
}

// delete an element from the tree
void deleteRoot(int array[], int num)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (num == array[i])
        {
            break;
        }
    }

    swap(&array[i], &array[size - 1]);
    size -= 1;
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heapify(array, size, i);
    }
}

// ---------------------------------------------
// END PRIORITY QUEUE

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