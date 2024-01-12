#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

#include "priority_queue.c"

node_t* map;
int mapWidth = 0, mapHeight = 0;

int RunDijkstras()
{
    // create a priority queue
    node_t* priorityQueue = (node_t*)malloc(mapHeight * mapWidth * sizeof(node_t));
    int priorityQueueSize = 0;

    node_t* queueOfSeenNodes = (node_t*)malloc(mapHeight * mapWidth * sizeof(node_t));
    int queueOfSeenNodesSize = 0;

    // add the starting node to the priority queue
    node_t* startingNode = &map[0];
    startingNode->totalHeatLoss = 0;
    InsertToPQ(priorityQueue, &priorityQueueSize, startingNode);

    // while the priority queue is not empty
    while (priorityQueueSize > 0)
    {
        // get the node with the lowest total heat loss
        node_t* currentNode = &priorityQueue[0];
        DeleteFromPQ(priorityQueue, &priorityQueueSize, 0);

        // if the node is the ending node, return the total heat loss
        if (currentNode->mapX == mapWidth - 1 && currentNode->mapY == mapHeight - 1)
        {
            return currentNode->totalHeatLoss;
        }

        // if the node is already in the queue of seen nodes, skip it
        if (IsSeenNode(queueOfSeenNodes, queueOfSeenNodesSize, currentNode))
        {
            continue;
        }

        InsertToPQ(queueOfSeenNodes, &queueOfSeenNodesSize, currentNode);

        if (currentNode->numSameDirection < 3 & (currentNode->direction.directionX != 0 || currentNode->direction.directionY != 0))
        {
            int directionX = currentNode->mapX + currentNode->direction.directionX;
            int directionY = currentNode->mapY + currentNode->direction.directionY;
            if (directionX < 0 || directionX >= mapWidth || directionY < 0 || directionY >= mapHeight)
            {
                continue;
            }
            node_t* newNode = &map[directionY * mapWidth + directionX];
            newNode->totalHeatLoss = currentNode->totalHeatLoss + newNode->coolingValue;
            newNode->direction.directionX = directionX;
            newNode->direction.directionY = directionY;
            newNode->numSameDirection++;
            InsertToPQ(priorityQueue, &priorityQueueSize, newNode);
        }
        // for each neighbor of the node
        for (int i = 0; i < 4; i++)
        {
            int neighborX = currentNode->mapX + (i == 0) - (i == 1);
            int neighborY = currentNode->mapY + (i == 2) - (i == 3);
            if (neighborX < 0 || neighborX >= mapWidth || neighborY < 0 || neighborY >= mapHeight)
            {
                continue;
            }

            if ((neighborX != currentNode->direction.directionX || neighborY != currentNode->direction.directionY) && (neighborX != currentNode->direction.directionX || neighborY != currentNode->direction.directionY))
            {
                node_t* newNode = &map[neighborY * mapWidth + neighborX];
                newNode->totalHeatLoss = currentNode->totalHeatLoss + newNode->coolingValue;
                newNode->direction.directionX = neighborX;
                newNode->direction.directionY = neighborY;
                newNode->numSameDirection = 1;
                InsertToPQ(priorityQueue, &priorityQueueSize, newNode);
            }
        }
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
    mapWidth = strlen(line);

    map = (node_t*)malloc(mapHeight * mapWidth * sizeof(node_t));
    rewind(input);
    for (int i = 0; i < mapHeight; i++)
    {
        fgets(line, sizeof(line), input);
        for (int j = 0; j < mapWidth; j++)
        {
            node_t* node = &map[i * mapWidth + j];
            node->mapX = j;
            node->mapY = i;
            node->coolingValue = line[j] - '0';
            printf("%d", node->coolingValue);
        }
        printf("\n");
    }
printf("\n");

    int totalHeatLoss = RunDijkstras();
    printf("Total Heat Loss: %d\n", totalHeatLoss);

    return 0;

}