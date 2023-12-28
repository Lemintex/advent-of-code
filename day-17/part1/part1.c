#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int** map;
int mapwidth = 0, mapheight = 0;
int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    while (fgets(line, sizeof(line), input))
    {
        mapheight++;
        /* code */
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