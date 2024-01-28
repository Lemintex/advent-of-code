#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LENGTH 14
typedef struct point
{
    long int x;
    long int y;
} point_t;

point_t points[LENGTH] = {0};

int main()
{
    int minX = 0, minY = 0;
    int maxX = 0, maxY = 0;
    int x = 0, y = 0;

    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];
    int pointIndex = 0;

    while (fgets(line, sizeof(line), input))
    {
        int length = 0;
        char* c = strchr(line, '#');
        c++;
        for (int i = 0; i < 5; i++)
        {
            int num = 0;
            if (c[i] >= '0' && c[i] <= '9')
            {
                num = c[i] - '0';
            }
            else if (c[i] >= 'a' && c[i] <= 'f')
            {
                num = c[i] - 'a' + 10;
            }
            length *= 16;
            length += num; 
            printf("%c", c[i]);
        }
        printf(" %c", c[5]);
        switch (c[5])
        {
            case '0': // right
                x += length;
                break;

            case '2': // left
                x -= length;
                break;

            case '3': // up
                y -= length;
                break;

            case '1': // down
                y += length;
                break;
        }
        points[pointIndex++] = (point_t){x, y};

        printf(" %d\n", length);
        printf("\n");
        if (x < minX) minX = x;
        if (y < minY) minY = y;
        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
    }

    // we need to ensure that all points are positive for gausses area formula
    // this is done by subtracting the min x and y from all points
    for (int i = 0; i < LENGTH; i++)
    {
        
        points[i].x -= minX;
        //points[i].x += 1000; // add 1000 to ensure no negative numbers
        points[i].y -= minY;
        //points[i].y += 1000; // add 1000 to ensure no negative numbers
    }

    long int area = 0;
    for (int i = 0; i < LENGTH - 1; i++)
    {
        area += points[i].x * points[i + 1].y - points[i + 1].x * points[i].y;
    }
    long int temp = points[LENGTH - 1].x * points[0].y - points[0].x * points[LENGTH - 1].y;
    area += points[LENGTH - 1].x * points[0].y - points[0].x * points[LENGTH - 1].y;
    area = labs(area) / 2;
    printf("Clockwise area: %ld\n", area);

    area = 0;
    long int perimeter = 0;
    for (int i = 0; i < LENGTH - 1; i++)
    {
        area -= points[i].x * points[(i+1)%LENGTH].y;
        area += points[i].y * points[(i+1)%LENGTH].x;
        perimeter += labs(points[i].x - points[(i+1)%LENGTH].x) + labs(points[i].y - points[(i+1)%LENGTH].y);
    }
    area = labs(area) / 2;
    printf("Counter clockwise area: %ld\n", area);
    printf("Grid points: %ld\n", area + perimeter - 1);
    return 0;
}
// 1048840725 is too low