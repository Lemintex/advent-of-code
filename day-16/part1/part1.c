#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** map;
int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    int mapHeight = 0;
    while (fgets(line, sizeof(line), input))
    {
        mapHeight++;
    }
    rewind(input);

    // allocate the map
    map = malloc(sizeof(char*) * mapHeight);
    for (int i = 0; fgets(line, sizeof(line), input); i++)
    {
        map[i] = malloc(sizeof(char) * strlen(line));
        strcpy(map[i], line);
    }
    for (int i = 0; i<mapHeight; i++)
    {
        printf("%s", map[i]);
    }
    
}