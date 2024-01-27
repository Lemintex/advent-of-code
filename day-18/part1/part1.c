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

    for (int i = 0; i < 410; i++)
    {
        for (int j = 0; j < 410; j++)
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

    x = -minX, y = -minY;
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

    int total = 0;
    for (int i = 0; i < -minY + maxY + 1; i++)
    {
        bool inside = false;
        if (map[i][0] == '#')
        {
            inside = true;
        }
        
        for (int j = 0; j < -minX + maxX + 1; j++)
        {
            if (map[i][j] == '#')
            {
                total++;
                if ((i > 0 && map[i-1][j] == '#') && (i < -minX + maxX && map[i+1][j] == '#'))
                {
                    inside = !inside;
                }
                else if (i > 0 && map[i-1][j] == '#')
                {
                    // inside = false;
                }
                else if (i < -minX + maxX && map[i+1][j] == '#')
                {
                    inside = !inside;
                }
            }
            // else if (inside)
            // {
            //     map[i][j] = 'X';
            // }
        }
    }
    FILE* output = fopen("../output.txt", "w");

        for (int i = 0; i < -minY + maxY + 1; i++)
    {
        for (int j = 0; j < -minX + maxX + 1; j++)
        {
            fprintf(output, "%c", map[i][j]);
            printf("%c", map[i][j]);
        }
        fprintf(output, "\n");
        printf("\n");
    }
    printf("Total: %d\n", total);
    fclose(input);
    return 0;
}
//40453 is too low