#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

#include "priority_queue.c"

node_t* map;
int mapWidth = 0, mapHeight = 0;

void AddSeenNode(node_t** queueOfSeenNodes, int* queueOfSeenNodesSize, node_t node)
{
    (*queueOfSeenNodes)[*queueOfSeenNodesSize] = node;
    (*queueOfSeenNodesSize)++;
    //*queueOfSeenNodes = (node_t*)realloc(*queueOfSeenNodes, sizeof(node_t) * (*queueOfSeenNodesSize));
}

bool IsSeenNode(node_t* queueOfSeenNodes, int queueOfSeenNodesSize, node_t node)
{
    for (int i = 0; i < queueOfSeenNodesSize; i++)
    {
        node_t seenNode = queueOfSeenNodes[i];
        if (seenNode.x == node.x && seenNode.y == node.y && seenNode.directionTravelling.x == node.directionTravelling.x && seenNode.directionTravelling.y == node.directionTravelling.y && seenNode.consecutiveDirectionCount == node.consecutiveDirectionCount)
        {
            return true;
        }
    }
    return false;
}

int RunDijkstras()
{
    // create a priority queue
    InitializePQ();

    node_t* queueOfSeenNodes = (node_t*)malloc(10000 * sizeof(node_t));
    int queueOfSeenNodesSize = 0;

    // add the starting node to the priority queue
    node_t startingNode = map[0];
    startingNode.totalHeatLoss = 0;
    Enqueue(startingNode);

    // while the priority queue is not empty
    while (!IsEmpty())
    {
        // get the node with the lowest total heat loss
        node_t currentNode = Dequeue();

        if (currentNode.x < 0 || currentNode.x > mapWidth || currentNode.y < 0 || currentNode.y > mapHeight)
        {
            continue;
        }

        // if the node is the ending node, return the total heat loss
        if (currentNode.x == mapWidth - 1 && currentNode.y == mapHeight - 1)
        {
            return currentNode.totalHeatLoss;
        }

        // if the node is already in the queue of seen nodes, skip it
        if (IsSeenNode(queueOfSeenNodes, queueOfSeenNodesSize, currentNode))
        {
            continue;
        }

        // if not seen, add it to the queue of seen nodes
        AddSeenNode(&queueOfSeenNodes, &queueOfSeenNodesSize, currentNode);

        if (currentNode.consecutiveDirectionCount < 3 && currentNode.directionTravelling.x != 0 && currentNode.directionTravelling.y != 0)
        {
            int newX = currentNode.x + currentNode.directionTravelling.x;
            int newY = currentNode.y + currentNode.directionTravelling.y;
            if (newX < 0 || newX > mapWidth || newY < 0 || newY > mapHeight)
            {
                continue;
            }
            node_t newNode = map[newY * mapWidth + newX];
            newNode.totalHeatLoss = currentNode.totalHeatLoss + newNode.heatLoss;
            newNode.directionTravelling.x = currentNode.directionTravelling.x;
            newNode.directionTravelling.y = currentNode.directionTravelling.y;
            newNode.consecutiveDirectionCount = currentNode.consecutiveDirectionCount + 1;
            Enqueue(newNode);
        }

        for (int i = 0; i < 4; i++)
        {
            int neighborX = currentNode.x + (i == 0) - (i == 1);
            int neighborY = currentNode.y + (i == 2) - (i == 3);
            
            int neighbourDirectionX = neighborX - currentNode.x;
            int neighbourDirectionY = neighborY - currentNode.y;
            if (neighborX < 0 || neighborX >= mapWidth || neighborY < 0 || neighborY >= mapHeight)
            {
                continue;
            }

            if (neighbourDirectionX != currentNode.directionTravelling.x || neighbourDirectionY != currentNode.directionTravelling.y && (neighbourDirectionX != -currentNode.directionTravelling.x || neighbourDirectionY != -currentNode.directionTravelling.y))
            {
                node_t newNode = map[neighborY * mapWidth + neighborX];
                newNode.totalHeatLoss = currentNode.totalHeatLoss + newNode.heatLoss;
                newNode.directionTravelling.x = neighbourDirectionX;
                newNode.directionTravelling.y = neighbourDirectionY;
                newNode.consecutiveDirectionCount = 0;
                Enqueue(newNode);
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
            node->x = j;
            node->y = i;
            node->heatLoss = line[j] - '0';
            printf("%d", node->heatLoss);
        }
        printf("\n");
    }
printf("\n");

    int totalHeatLoss = RunDijkstras();
    printf("Total Heat Loss: %d\n", totalHeatLoss);

    return 0;
}