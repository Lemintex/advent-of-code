#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** map;

int main()
{
    int maxX = 0, maxY = 0;
    int x = 0, y = 0;

    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    while (fgets(line, sizeof(line), input))
    {
        switch (line[0])
        {
            case 'R':
                x += atoi(&line[2]);
                break;

            case 'L':
                x -= atoi(&line[2]);
                break;

            case 'U':
                y -= atoi(&line[2]);
                break;

            case 'D':
                y += atoi(&line[2]);
                break;
        }
        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
    }
    rewind(input);
    map = (char**)malloc((maxY + 1) * sizeof(char*));
    for (int i = 0; i < maxY + 1; i++)
    {
        map[i] = (char*)malloc((maxX + 1) * sizeof(char));
        memset(map[i], '.', maxX + 1);
    }

    x = 0, y = 0;
    while (fgets(line, sizeof(line), input))
    {
        switch (line[0])
        {
            case 'R':
                for (int i = 0; i < atoi(&line[2]); i++)
                {
                    x++;
                    map[y][x] = '#';
                }
                break;

            case 'L':
                for (int i = 0; i < atoi(&line[2]); i++)
                {
                    x--;
                    map[y][x] = '#';
                }
                break;

            case 'U':
                for (int i = 0; i < atoi(&line[2]); i++)
                {
                    y--;
                    map[y][x] = '#';
                }
                break;

            case 'D':
                for (int i = 0; i < atoi(&line[2]); i++)
                {
                    y++;
                    map[y][x] = '#';
                }
                break;
        }
    }
    for (int i = 0; i < maxY + 1; i++)
    {
        for (int j = 0; j < maxX + 1; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
    fclose(input);
    return 0;
}