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

void FindShortestPath()
{

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
            node->coolingValue = line[j] - '0';
            node->pathValue = 999999;
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