#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAP_SIZE 140

char map[MAP_SIZE][MAP_SIZE] = {0};
bool visited[MAP_SIZE][MAP_SIZE] = {false};

bool IsTouchingSymbol(int x, int y)
{
    for (int xIndex = x - 1; xIndex <= x + 1; xIndex++) {
        for (int yIndex = y - 1; yIndex <= y + 1; yIndex++) {
            if (xIndex == x && yIndex == y) continue;
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
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            char* firstDigit = strpbrk(&map[y][x], "0123456789");
            if (firstDigit >= &map[MAP_SIZE * y + 1][0] || firstDigit == NULL) break;
            char* lastDigit = firstDigit + strspn(firstDigit, "0123456789");
            if (lastDigit > &map[MAP_SIZE * y + 1][0]) lastDigit = map[MAP_SIZE * y + 1];

            char number[10];
            strncpy(number, firstDigit, lastDigit - firstDigit);
            int n = atoi(number);

            if (n == 0) continue;
            bool isTouchingSymbol;
            for (int posX = x; posX < x + (lastDigit - firstDigit); posX++) {
                isTouchingSymbol |= IsTouchingSymbol(posX, y);
            }
            if (isTouchingSymbol)
            {
                total += n;
            }
        }
    }
    printf("%d", total);
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