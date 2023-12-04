#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX 2048
#define MAP_SIZE 140

typedef struct {
    int x;
    int y;
} symbol_t;

char map[MAP_SIZE][MAP_SIZE] = {0};
symbol_t symbols[MAX] = {0};

bool IsTouchingSymbol(int x, int y)
{
    for (int xIndex = x - 1; xIndex <= x + 1; xIndex++) {
        for (int yIndex = y - 1; yIndex <= y + 1; yIndex++) {
            if (xIndex == x && yIndex == y) continue;
            if (xIndex < 0 || xIndex >= MAP_SIZE || yIndex < 0 || yIndex >= MAP_SIZE) continue;
            char c = map[yIndex][xIndex];
            if (c != '.' && !(c >= '0' && c <= '9')) {
                return true;
            }
        }
    }
    return false;
}
void CheckMap()
{
    int total = 0;
    for (int y = 0; y < MAP_SIZE; y++)
    {
        char* pos = map[y];
        while (true)
        {
            char* firstDigit = strpbrk(pos, "0123456789");
            if (firstDigit >= map[MAP_SIZE * y + 1] || firstDigit == NULL)
            {
                break;
            }

            char* lastDigit = firstDigit + strspn(firstDigit, "0123456789");

            char number[10] = {0};
            strncpy(number, firstDigit, lastDigit - firstDigit);
            int n = atoi(number);
            pos = lastDigit;

            if (n == 0) continue;
            int x = firstDigit - map[y];
            bool isTouchingSymbol = false;
            for (int posX = x; posX < x + (lastDigit - firstDigit); posX++) {
                isTouchingSymbol |= IsTouchingSymbol(posX, y);
            }
            if (isTouchingSymbol)
            {
                total += n;
            }
            else
            {
                //printf("Symbol %d is not touching anything\n", n);
            }
        }
    }
    printf("Total: %d", total);
}

int main()
{
    FILE* input = fopen("../input.txt", "r");

    int total = 0;
    char line[256];
    
    int y = 0;
    for (int i = 0; fgets(line, 256, input) != NULL; i++) {
        memcpy(&map[i], line, MAP_SIZE);
    }
    
    CheckMap();

    printf("%d", total);
}

// go through each line
// for each character
// check if symbol