#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX 2048
#define MAP_SIZE 140
int gearX = 0, gearY = 0;
//can't have 3 gears touching each other
typedef struct {
    int x;
    int y;
    int parts[2];
    int gears;//rename
    bool isValid;
} gears_t;

char map[MAP_SIZE][MAP_SIZE] = {0};
gears_t gears[MAX];
int gearsCount = -1;

void AddToNewGear(int n)
{
    gearsCount++;
    gears[gearsCount].x = gearX;
    gears[gearsCount].y = gearY;
    gears[gearsCount].parts[0] = n;
    gears[gearsCount].gears = 1;
    gears[gearsCount].isValid = true;
}

void AddToExistingGearInfo(int n, int index)
{
    gears[index].parts[1] = n;
    gears[index].gears = 2;
}

void CheckExistingGearInfo(int n)
{
    for (int i = 0; i <= gearsCount; i++) {
        if (gears[i].x == gearX && gears[i].y == gearY) {
            if (gears[i].gears == 2 && gears[i].isValid)
            {
                gears[i].isValid = false;
            }
            else
            {
                AddToExistingGearInfo(n, i);
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
    for (int i = 0; i <= gearsCount; i++) {
        if (gears[i].isValid && gears[i].gears == 2) {
            total += gears[i].parts[0] * gears[i].parts[1];
            printf("%d * %d = %d\n", gears[i].parts[0], gears[i].parts[1], gears[i].parts[0] * gears[i].parts[1]);
        }
    }
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