#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int Evaluate();
typedef enum
{
    NORTH,
    EAST,
    SOUTH,
    WEST
} direction_t;

typedef struct
{
    char tile;
    bool visited;
} tile_t;

tile_t** map;
int mapWidth = 0, mapHeight = 0;

void UpdateXY(int* x, int* y, direction_t direction)
{
    switch (direction)
    {
        case NORTH:
            (*y)--;
            break;
        case EAST:
            (*x)++;
            break;
        case SOUTH:
            (*y)++;
            break;
        case WEST:
            (*x)--;
            break;
    }
}

void Step(int x, int y, direction_t direction)
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
    {
        return;
    }

    char tile = map[y][x].tile;
    
    if ((tile == '|' || tile == '-') && map[y][x].visited)
    {
        return;
    }

    map[y][x].visited = true;
    switch (tile)
    {
        case '/' :
            switch (direction)
            {
                case NORTH:
                    direction = EAST;
                    break;

                case EAST:
                    direction = NORTH;
                    break;

                case SOUTH:
                    direction = WEST;
                    break;

                case WEST:
                    direction = SOUTH;
                    break;
            }
            UpdateXY(&x, &y, direction);
            Step(x, y, direction);
            break;

        case '\\' :
            switch (direction)
            {
                case NORTH:
                    direction = WEST;
                    break;

                case EAST:
                    direction = SOUTH;
                    break;

                case SOUTH:
                    direction = EAST;
                    break;

                case WEST:
                    direction = NORTH;
                    break;
            }
            UpdateXY(&x, &y, direction);
            Step(x, y, direction);
            break;

        case '|' :
            switch (direction)
            {
                case NORTH:
                case SOUTH:
                    UpdateXY(&x, &y, direction);
                    Step(x, y, direction);
                    break;

                case EAST:
                case WEST:
                    int x1 = x, y1 = y;
                    int x2 = x, y2 = y;
                    UpdateXY(&x1, &y1, NORTH);
                    UpdateXY(&x2, &y2, SOUTH);
                    Step(x1, y1, NORTH);
                    Step(x2, y2, SOUTH);
                    return;
            }
            break;

        case '-' :
            switch (direction)
            {
                case NORTH:
                case SOUTH:
                    int x1 = x, y1 = y;
                    int x2 = x, y2 = y;
                    UpdateXY(&x1, &y1, EAST);
                    UpdateXY(&x2, &y2, WEST);
                    Step(x1, y1, EAST);
                    Step(x2, y2, WEST);
                    return;

                case EAST:
                case WEST:
                    UpdateXY(&x, &y, direction);
                    Step(x, y, direction);
                    break;
            }
            break;

        case '.' :
            UpdateXY(&x, &y, direction);
            Step(x, y, direction);
            break;
    }
}

void ResetMap()
{
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            map[i][j].visited = false;
        }
    }
}

// returns the maximum number of tiles visited from all possible starting points
int Simulate()
{
    int max = 0;
    // horizontal
    for (int i = 0; i < mapHeight; i++)
    {
        ResetMap();
        int x = 0, y = i;
        direction_t direction = EAST;
        Step(x, y, direction);
        if (Evaluate() > max)
        {
            max = Evaluate();
        }
        ResetMap();
        x = mapWidth - 1, y = i;
        direction = WEST;
        Step(x, y, direction);
        if (Evaluate() > max)
        {
            max = Evaluate();
        }
    }

    // vertical
    for (int i = 0; i < mapWidth; i++)
    {
        ResetMap();
        int x = i, y = 0;
        direction_t direction = SOUTH;
        Step(x, y, direction);
        if (Evaluate() > max)
        {
            max = Evaluate();
        }
        ResetMap();
        x = i, y = mapHeight - 1;
        direction = NORTH;
        Step(x, y, direction);
        if (Evaluate() > max)
        {
            max = Evaluate();
        }
    }

    return max;
}

int Evaluate()
{
    int total = 0;
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            if (map[i][j].visited)
            {
                total++;
            }
        }
    }
    return total;
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
    rewind(input);

    // allocate the memory for the map
    map = (tile_t**)malloc(sizeof(tile_t*) * mapHeight);

    for (int i = 0; fgets(line, sizeof(line), input); i++)
    {
        map[i] = (tile_t*)malloc(sizeof(tile_t) * strlen(line));
        for (int j = 0; j < strlen(line); j++)
        {
            map[i][j].visited = false;
            map[i][j].tile = line[j];
        }
    }

    mapWidth = strlen(line);

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            char tile = map[i][j].tile;
        }
    }

    printf("Total: %d\n", Simulate());
}