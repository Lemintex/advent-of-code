#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

typedef enum
{
    NORTH,
    EAST,
    SOUTH,
    WEST
} direction_t;

typedef struct
{
    int value;
    direction_t direction;
    bool visited;
} node_t;

int** map;
int mapwidth = 0, mapheight = 0;

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
        mapheight++;
    }
    mapwidth = strlen(line) - 1;
    map = (int**)malloc(mapheight * sizeof(int*));
    rewind(input);
    for (int i = 0; i < mapheight; i++)
    {
        fgets(line, sizeof(line), input);
        map[i] = (int*)malloc(mapwidth * sizeof(int));
        for (int j = 0; j < mapwidth; j++)
        {
            map[i][j] = line[j] - '0';
        }
    }

    for (int i = 0; i < mapheight; i++)
    {
        for (int j = 0; j < mapwidth; j++)
        {
            printf("%d", map[i][j]);
        }
        printf("\n");
    }
    
}