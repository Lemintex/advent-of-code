#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

typedef struct direction {
    int x;
    int y;
} direction_t;
typedef struct node node_t;

typedef struct node {
    int x, y;
    int heatLoss;
    int totalHeatLoss;
    direction_t directionTravelling;
    int consecutiveDirectionCount;

    bool isPath;
    node_t* previousNode;
} node_t;

node_t* pq;
int pqSize = 0;

node_t* seenNodes;
int seenNodesSize = 0;

node_t* map;
int mapWidth = 0, mapHeight = 0;

void AddToPQ(node_t node)
{
    pqSize++;
    pq[pqSize] = node;
}

node_t GetMinFromPQ()
{
    int minIndex = 0;
    node_t minNode = pq[0];
    for (int i = 0; i < pqSize; i++)
    {
        if (pq[i].totalHeatLoss < minNode.totalHeatLoss)
        {
            minIndex = i;
            minNode = pq[i];
        }
    }
    printf("PQ before: ");
    for (int i = 0; i < pqSize; i++)
    {
    printf("%d ", pq[i].totalHeatLoss);
    }
    for (int i = minIndex; i < pqSize; i++)
    {
        pq[i] = pq[i + 1];
    }
    pqSize--;
    printf("\nPQ after: ");
    for (int i = 0; i < pqSize; i++)
    {
    printf("%d ", pq[i].totalHeatLoss);
    }
    //pq = (node_t*)realloc(pq, pqSize * sizeof(node_t));
    return minNode;
}

void AddSeenNode(node_t node)
{
    seenNodesSize += 1;
    //seenNodes = (node_t*)realloc(seenNodes, seenNodesSize * sizeof(node_t));
    seenNodes[seenNodesSize - 1] = node;
}

bool IsSeenNode(node_t node)
{
    for (int i = 0; i < seenNodesSize; i++)
    {
        node_t seenNode = seenNodes[i];
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
    pq = (node_t*)malloc(10000 * sizeof(node_t));
    pqSize = 0;

    // create a queue of seen nodes
    seenNodes = (node_t*)malloc(10000 * sizeof(node_t));
    seenNodesSize = 0;

    // add the starting node to the priority queue
    node_t startingNode = map[0];
    map[0].previousNode = NULL;
    startingNode.totalHeatLoss = 0;

    AddToPQ(startingNode);

    while (pqSize > 0)
    {
        // get the node with the lowest total heat loss
        node_t currentNode = GetMinFromPQ();

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
        if (IsSeenNode(currentNode))
        {
            continue;
        }

        // if not seen, add it to the queue of seen nodes
        AddSeenNode(currentNode);

        // add the neighbors to the priority queue
        for (int i = 0; i < 4; i++)
        {
            int neighborX = currentNode.x + (i == 0) - (i == 1);
            int neighborY = currentNode.y + (i == 2) - (i == 3);
            
            int neighbourDirectionX = neighborX - currentNode.x;
            int neighbourDirectionY = neighborY - currentNode.y;
            // if (neighborX < 0 || neighborX >= mapWidth || neighborY < 0 || neighborY >= mapHeight)
            // {
            //     continue;
            // }

            // if (neighbourDirectionX == -currentNode.directionTravelling.x && neighbourDirectionY == -currentNode.directionTravelling.y)
            // {
            //     continue;
            // }
            // else if (neighbourDirectionX == currentNode.directionTravelling.x && neighbourDirectionY == currentNode.directionTravelling.y)
            // {
            //     currentNode.consecutiveDirectionCount++;
            // }
            // else
            // {
            //     currentNode.consecutiveDirectionCount = 0;
            // }

            //if (neighbourDirectionX != currentNode.directionTravelling.x || neighbourDirectionY != currentNode.directionTravelling.y && (neighbourDirectionX != -currentNode.directionTravelling.x || neighbourDirectionY != -currentNode.directionTravelling.y))
            //{
                map[neighborY * mapWidth + neighborX].previousNode = &map[currentNode.y * mapWidth + currentNode.x];

                node_t newNode = map[neighborY * mapWidth + neighborX];
                newNode.x = neighborX;
                newNode.y = neighborY;
                newNode.totalHeatLoss = currentNode.totalHeatLoss + newNode.heatLoss;
                newNode.directionTravelling.x = neighbourDirectionX;
                newNode.directionTravelling.y = neighbourDirectionY;
                newNode.consecutiveDirectionCount = 0;
                AddToPQ(newNode);
            //}
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
    map[0].previousNode = NULL;
    node_t* currentNode = &map[mapHeight * mapWidth - 1];
    int c = 0;
    while ((currentNode->x != 0 || currentNode->y != 0) && c++ < 1000)
    {
        currentNode->isPath = true;
        currentNode = currentNode->previousNode;
    }

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            node_t node = map[i * mapWidth + j];
            if (node.isPath)
            {
                printf("X");
            }
            else
            {
                printf("%d", node.heatLoss);
            }
        }
        printf("\n");
    }
    return 0;
}

//while the priority queue is not empty
//    get the node with the lowest total heat loss from the priority queue

//    if the node is the ending node, return the total heat loss

//    if the node is already in the queue of seen nodes, skip it

//    if not seen, add it to the queue of seen nodes so we don't process it again

//    add the neighbors to the priority queue
//        if the neighbor is the same direction as the current node
//            increment the consecutive direction count if <3 and add the neighbor to the priority queue with the total heat loss of the current node + the heat loss of the neighbor
//        if the neighbor is not the opposite direction of the current node (left, right)
//            add the neighbor to the priority queue with the total heat loss of the current node + the heat loss of the neighbor