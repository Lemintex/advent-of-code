#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX 2048
#define MAP_SIZE 140
int gearX = 0, gearY = 0;
typedef struct {
    int x;
    int y;
    int parts[2];
    int gears;
    bool isValid;
} gears_t;

char map[MAP_SIZE][MAP_SIZE] = {0};
gears_t gears[MAX] = {0};
int gearsCount = -1;

void AddToNewGear(int n)
{
    gearsCount++;
    gears[gearsCount].x = gearX;
    gears[gearsCount].y = gearY;
    gears[gearsCount].parts[0] = n;
    gears[gearsCount].gears++;
    gears[gearsCount].isValid = true;
}

void AddToExistingGearInfo(int n)
{
    gears[gearsCount].parts[1] = n;
    gears[gearsCount].gears++;
}

void CheckExistingGearInfo(int n)
{
    for (int i = 0; i < gearsCount; i++) {
        if (gears[i].x == gearX && gears[i].y == gearY) {
            if (!gears[i].isValid || gears[i].gears == 2)
            {
                gears[i].isValid = false;
            }
            else
            {
                AddToExistingGearInfo(n);
            }
            return;
        }
    }
    AddToNewGear(n);
}

bool IsTouchingGear(int x, int y)
{
    for (int xIndex = x - 1; xIndex <= x + 1; xIndex++) {
        for (int yIndex = y - 1; yIndex <= y + 1; yIndex++) {
            if (xIndex == x && yIndex == y) continue;
            if (xIndex < 0 || xIndex >= MAP_SIZE || yIndex < 0 || yIndex >= MAP_SIZE) continue;
            char c = map[yIndex][xIndex];
            if (c == '*') {
                gearX = xIndex;
                gearY = yIndex;
                return true;
            }
        }
    }
    return false;
}

void CalculateGearRatio()
{
    int total = 0;
    for (int i = 0; i < gearsCount; i++) {
        if (gears[i].isValid && gears[i].gears == 2) {
            total += gears[i].parts[0] * gears[i].parts[1];
        }
    }
    printf("Total: %d", total);
}
void CheckMap()
{
    int total = 0;
    for (int y = 0; y < MAP_SIZE; y++)
    {
        char* pos = map[y];
        char* lastPos = map[y+1];
        while (true)
        {
            char* firstDigit = strpbrk(pos, "0123456789");
            if (firstDigit >= map[y + 1] || firstDigit == NULL)
            {
                break;
            }

            char* lastDigit = firstDigit + strspn(firstDigit, "0123456789");
            int length = 0;
            if (lastDigit >= map[y + 1] || lastDigit == NULL)
            {
                lastDigit = map[y + 1] - 1;
                length = lastDigit - firstDigit;
                length++;
            }
            else
            {
                length = lastDigit - firstDigit;
            }

            char number[10] = {0};
            strncpy(number, firstDigit, length);
            int n = atoi(number);
            pos = lastDigit;

            if (n == 0) continue;
            int x = firstDigit - map[y];
            bool isTouchingGear = false;

            for (int posX = x; posX < x + length; posX++) {
                isTouchingGear = IsTouchingGear(posX, y);
                if (isTouchingGear)
                {
                    CheckExistingGearInfo(n);
                }
                if (isTouchingGear) break;
            }
            if (lastDigit >= lastPos - 1)
            {
                break;
            }
        }
    }
    CalculateGearRatio();
    printf("Total: %d", total);
}

int main()
{
    FILE* input = fopen("../input.txt", "r");

    char line[256];
    
    int y = 0;
    for (int i = 0; fgets(line, 256, input) != NULL; i++) {
        memcpy(&map[i], line, MAP_SIZE);
    }
    
    for (int i = 0; i < MAX; i++) {
        gears[i].x = 0;
        gears[i].y = 0;
        gears[i].parts[0] = 0;
        gears[i].parts[1] = 0;
        gears[i].gears = 0;
        gears[i].isValid = false;
    }
    CheckMap();
}