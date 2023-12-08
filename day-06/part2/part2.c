#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUM_RACES 4 
typedef struct
{
    long int time;
    long int distanceToBeat;
} race_t;

race_t race = {0};

void CalculateTime()
{
    int waysToWin = 0;
    int min = 0, max = 0;
    for (int t = 0; t < race.time; t++)
    {
        long int speed = t;
        long int timeToMove = race.time - t;
        long int distance =  timeToMove * speed;
        if (distance > race.distanceToBeat)
        {
            min = t;
            printf("Min: %d\n", min);
            break;
        }
    }

    for (int t = race.time - 1; t > 0; t--)
    {
        long int speed = t;
        long int timeToMove = race.time - t;
        long int distance =  timeToMove * speed;
        if (distance > race.distanceToBeat)
        {
            max = t;
            printf("Max: %d\n", max);
            break;
        }
    }
    printf("Way to win: %d\n", max - min + 1);
}

void ParseInput(char* lineContents, int lineNumber)
{
    if (lineNumber == 0)
    {
        char* firstDigit = strpbrk(lineContents, "0123456789");
        if (firstDigit == NULL) return;
        char* lastDigit = firstDigit + strspn(firstDigit, "0123456789");
        int length = lastDigit - firstDigit;
        char number[20] = {0};
        strncpy(number, firstDigit, length);
        race.time = atoi(number);
        lineContents = lastDigit;
    }
    else if (lineNumber == 1)
    {
        char* firstDigit = strpbrk(lineContents, "0123456789");
        if (firstDigit == NULL) return;
        char* lastDigit = firstDigit + strspn(firstDigit, "0123456789");
        int length = lastDigit - firstDigit;
        char number[20] = {0};
        strncpy(number, firstDigit, length);
        race.distanceToBeat = atol(number);
        lineContents = lastDigit;
    }
}

int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];
    int l = 0;
    int i = 0;
    while (fgets(line, sizeof(line), input))
    {
        char parsed[256] = {0};
        for (int i = 0, j = 0; i < 256; i++)
        {
            if (line[i] >= '0' && line[i] <= '9')
            {
                parsed[j] = line[i];
                j++;
            }
            parsed[i] = line[i];
        }
        ParseInput(parsed, l++);
    }
CalculateTime();

    }

