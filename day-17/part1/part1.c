#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

typedef enum
{
    NONE,
    NORTH,
    EAST,
    SOUTH,
    WEST
} direction_t;

typedef struct node_t node_t;
struct node_t
{
    node_t* parent;
    bool isStartNode;
    bool isPath;
    int mapX, mapY;
    int coolingValue;
    int totalPathValue;
    direction_t direction;
    int numSameDirection;
    bool queued;
    bool visited;
};

node_t** visitedNodes;

node_t* nodesByPathValue;
node_t** map;
int mapWidth = 0, mapHeight = 0;

void Dijkstra() {
    // put 2d array into 1d array
    node_t* nodes[mapHeight * mapWidth];
    int index = 0;
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            nodes[index] = &map[i][j];
            nodes[index]->visited = false;
            index++;
        }
    }

    node_t* currentNode = &map[0][0];
    currentNode->isStartNode = true;
    currentNode->queued = true;
    currentNode->totalPathValue = currentNode->coolingValue;
    while (true)
    {
        printf("Current node direction: %d\n", currentNode->numSameDirection);
        if (currentNode->mapX == mapWidth - 1 && currentNode->mapY == mapHeight - 1)
        {
            printf("Shortest path: %d\n", currentNode->totalPathValue);
            for (int i = 0; i < mapWidth; i++)
            {
                printf("%d", map[mapHeight - 1][i].coolingValue);
            }
            printf("\n");
            printf("Cooling value: %d\n", currentNode->coolingValue);
            return;
        }
        printf("Node path value: %d\n", currentNode->totalPathValue);
        // Find the next node to visit
        int minPathValue = 999999999;
        int minPathValueIndex = 0;
        for (int i = 0; i < mapHeight * mapWidth; i++)
        {
            node_t* node = nodes[i];
            if (node->parent == NULL)
            {
                continue;
            }
            if (node->coolingValue + node->parent->totalPathValue < minPathValue && !node->visited && node->queued)
            {
                minPathValue = node->parent->totalPathValue + node->coolingValue;
                minPathValueIndex = i;
            }
        }
        currentNode = nodes[minPathValueIndex];

        currentNode->totalPathValue = minPathValue;
        currentNode->visited = true;
        

        // Visit all neighbors
        node_t* neighbors[4];
        int neighborsCount = 0;
        if (currentNode->mapX > 0)
        {
            neighbors[neighborsCount] = &map[currentNode->mapY][currentNode->mapX - 1];
            neighbors[neighborsCount]->direction = EAST;
            neighborsCount++;
        }

        if (currentNode->mapX < mapWidth - 1)
        {
            neighbors[neighborsCount] = &map[currentNode->mapY][currentNode->mapX + 1];
            neighbors[neighborsCount]->direction = WEST;
            neighborsCount++;
        }

        if (currentNode->mapY > 0)
        {
            neighbors[neighborsCount] = &map[currentNode->mapY - 1][currentNode->mapX];
            neighbors[neighborsCount]->direction = NORTH;
            neighborsCount++;
        }

        if (currentNode->mapY < mapHeight - 1)
        {
            neighbors[neighborsCount] = &map[currentNode->mapY + 1][currentNode->mapX];
            neighbors[neighborsCount]->direction = SOUTH;
            neighborsCount++;
        }

        for (int i = 0; i < neighborsCount; i++)
        {
            node_t* neighbor = neighbors[i];
            if (neighbor->visited || (neighbor->direction == currentNode->direction && currentNode->numSameDirection >= 2))
            {
                continue;
            }
            if (neighbor->direction != currentNode->direction)
            {
                neighbor->numSameDirection = 0;
            }
            else
            {
                neighbor->numSameDirection = currentNode->numSameDirection + 1;
            }
            neighbor->queued = true;
            neighbor->direction = currentNode->direction;
            neighbor->parent = &map[currentNode->mapY][currentNode->mapX];
            if (currentNode->parent != NULL)
            {
                printf("Parent node: %d, %d\n", currentNode->parent->mapX, currentNode->parent->mapY);
            }
            if (!neighbor->visited)
            {
                neighbor->totalPathValue = currentNode->totalPathValue + neighbor->coolingValue;
            }
            else
            {
                if (neighbor->totalPathValue > currentNode->totalPathValue + neighbor->coolingValue)
                {
                    neighbor->totalPathValue = currentNode->totalPathValue + neighbor->coolingValue;
                }
            }
        }
    }
    printf("Shortest path: %d\n", currentNode->totalPathValue);
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
    mapWidth = strlen(line);

    nodesByPathValue = (node_t*)malloc(mapHeight * mapWidth * sizeof(node_t));
    map = (node_t**)malloc(mapHeight * sizeof(node_t*));
    int nodeIndex = 0;
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
            node->totalPathValue = 99999999;
            node->visited = false;
            node->direction = NONE;
            map[i][j] = *node;

            nodesByPathValue[nodeIndex] = *node;
        }
    }


    Dijkstra();


node_t* currentNode = &map[mapHeight - 1][mapWidth - 1];
    while (!currentNode->isStartNode)
    {
        currentNode->isPath = true;
        currentNode = (node_t*)currentNode->parent;
    }
    for (int i = 0; i < mapHeight; i++)
    {
    for (int j = 0; j < mapWidth; j++)
        {
            if (map[i][j].isPath)
            {
                printf("X");
            }
            else
            {
                printf("%d", map[i][j].coolingValue);
            }
        }
        printf("\n");
    }

}