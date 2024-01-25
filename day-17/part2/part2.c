// this needs entirely rewritten, it's a mess and takes 4 minutes to run

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

typedef struct direction {
    int x;
    int y;
} direction_t;

typedef struct node {
    int x, y;
    int initialHeatLoss;
    direction_t facing;
    int stepsInSameDirection;
    int heatLoss;

    int prevX, prevY;
    bool ispartOfPath;
} node_t;

node_t pq[10000000];
int pqCount = 0;

node_t seenNodes[10000000];
int seenNodesCount = 0;

node_t* map;
int mapWidth = 0, mapHeight = 0;

void AddToSeenNodes(node_t* node)
{
    seenNodes[seenNodesCount++] = *node;
}

bool IsInSeenNodes(node_t node)
{
    for (int i = 0; i < seenNodesCount; i++)
    {
        if (seenNodes[i].x == node.x && seenNodes[i].y == node.y && seenNodes[i].facing.x == node.facing.x && seenNodes[i].facing.y == node.facing.y && seenNodes[i].stepsInSameDirection == node.stepsInSameDirection)
        {
            return true;
        }
    }
    return false;
}

void Enqueue(node_t node)
{
    pq[pqCount++] = node;
}

node_t Dequeue()
{
    int lowestHeatLoss = 999999;
    int lowestHeatLossIndex = 0;
    for (int i = 0; i < pqCount; i++)
    {
        if (pq[i].heatLoss < lowestHeatLoss)
        {
            lowestHeatLoss = pq[i].heatLoss;
            lowestHeatLossIndex = i;
        }
    }
    node_t node = pq[lowestHeatLossIndex];
    for (int i = lowestHeatLossIndex; i < pqCount - 1; i++)
    {
        pq[i] = pq[i + 1];
    }
    pqCount--;
    return node;
}

int RunDijkstras()
{
    int hits = 0;
    node_t startNode = map[0];
    startNode.heatLoss = 0;
    Enqueue(startNode);
    while (pqCount > 0)
    {
        // get the node with the lowest total heat loss from the priority queue
        node_t node = Dequeue();

        // if the node is already in the queue of seen nodes, skip it
        if (IsInSeenNodes(node))
        {
            continue;
        }

        // if not seen, add it to the queue of seen nodes so we don't process it again
        AddToSeenNodes(&node);

        // for each neighbor
        for (int i = 0; i < 4; i++)
        {
            node_t neighbor = (node_t){ 0, 0, 0, (direction_t){ 0, 0 }, 0, 0, 0, 0, false };
            switch (i)
            {
                case 0:
                    if (node.x > 0)
                    {
                        hits++;
                    neighbor = map[(node.y * mapWidth) + (node.x - 1)];
                        neighbor.facing = (direction_t){ -1, 0 };
                    }
                    break;
                case 1:
                    if (node.x < mapWidth - 1)
                    {
                        neighbor = map[(node.y * mapWidth) + (node.x + 1)];
                        neighbor.facing = (direction_t){ 1, 0 };
                    }
                    break;
                case 2:
                    if (node.y > 0)
                    {
                        neighbor = map[((node.y - 1) * mapWidth) + node.x];
                        neighbor.facing = (direction_t){ 0, -1 };
                    }
                    break;
                case 3:
                    if (node.y < mapHeight - 1)
                    {
                        neighbor = map[((node.y + 1) * mapWidth) + node.x];
                        neighbor.facing  = (direction_t){ 0, 1 };
                    }
                    break;
            }

            if ((neighbor.x == -1 && neighbor.y == -1) || (neighbor.facing.x == -node.facing.x && neighbor.facing.y == -node.facing.y) || IsInSeenNodes(neighbor))
            {
                continue;
            }

            // if the neighbor is the same direction as the current node
            if (neighbor.facing.x == node.facing.x && neighbor.facing.y == node.facing.y || node.x == 0 && node.y == 0)
            {
                // increment the consecutive direction count if <3 and add the neighbor to the priority queue with the total heat loss of the current node + the heat loss of the neighbor
                if (node.stepsInSameDirection >= 10)
                {
                    continue;
                }
                neighbor.stepsInSameDirection = node.stepsInSameDirection + 1;
            }
            else
            {
                if (node.stepsInSameDirection < 4)
                {
                    continue;
                }
                neighbor.stepsInSameDirection = 1;
            }

            if (neighbor.x == mapWidth - 1 && neighbor.y == mapHeight - 1)
            {
                printf("Found the end node!\n");
                neighbor.heatLoss += node.heatLoss;
                printf("Total heat loss: %d\n", neighbor.heatLoss);
                return node.heatLoss;
            }

            neighbor.heatLoss += node.heatLoss;
            Enqueue(neighbor);
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
            node->initialHeatLoss = node->heatLoss;
            node->ispartOfPath = false;
        }
    }
    RunDijkstras();
}