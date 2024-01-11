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
    int mapX, mapY;
    int coolingValue;
    int totalHeatLoss;
    direction_t direction;
    int numSameDirection;
};

node_t* map;
node_t* seenNodes;
int mapWidth = 0, mapHeight = 0;

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
}