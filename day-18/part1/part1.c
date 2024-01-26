#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char map[400][400];

int main()
{
    int minX = 0, minY = 0;
    int maxX = 0, maxY = 0;
    int x = 0, y = 0;

    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    for (int i = 0; i < 400; i++)
    {
        for (int j = 0; j < 400; j++)
        {
            map[i][j] = '.';
        }
    }

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
        if (x < minX) minX = x;
        if (y < minY) minY = y;
        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
    }
    printf("minX: %d, minY: %d, maxX: %d, maxY: %d\n", minX, minY, maxX, maxY);
    rewind(input);
//    map = (char**)malloc((maxY + 1) - minY * sizeof(char*));
//    for (int i = 0; i < -minY + maxY + 1; i++)
//    {
//        map[i] = (char*)malloc((maxX + 1) * sizeof(char));
//        memset(map[i], '.', maxX + 1);
//    }

    x = 0, y = -minY;
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
    for (int i = 0; i < -minY + maxY + 1; i++)
    {
        for (int j = 0; j < maxX + 1; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
    int total = 0;
    for (int i = 0; i < maxY + 1; i++)
    {
        bool inside = false;
        for (int j = 0; j < maxX + 1; j++)
        {
            if (map[i][j] == '#')
            {
                total++;
            }
        }
    }
    printf("Total: %d\n", total);
    fclose(input);
    return 0;
}