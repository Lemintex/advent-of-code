#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUM_RACES 4 
typedef struct
{
    int time;
    int distanceToBeat;
} race_t;

race_t races[4] = {0};

void CalculateTime()
{
    int total = 1;
    for (int i = 0; i < NUM_RACES; i++)
    {
        int waysToWin = 0;
        for (int t = 0; t < races[i].time; t++)
        {
            int speed = t;
            int timeToMove = races[i].time - t;
            int distance =  timeToMove * speed;
            if (distance > races[i].distanceToBeat)
            {
                waysToWin++;
            }
        }
        printf("Way to win: %d\n", waysToWin);
        total *= waysToWin;
    }    
    printf("Total ways to win: %d\n", total);
}

void ParseInput(char* lineContents, int lineNumber)
{
    if (lineNumber == 0)
    {
        for (int r = 0; r < NUM_RACES; r++)
        {
            char* firstDigit = strpbrk(lineContents, "0123456789");
            if (firstDigit == NULL) return;
            char* lastDigit = firstDigit + strspn(firstDigit, "0123456789");
            int length = lastDigit - firstDigit;
            char number[10] = {0};
            strncpy(number, firstDigit, length);
            races[r].time = atoi(number);
            lineContents = lastDigit;
        }

    }
   else if (lineNumber == 1)
   {
        for (int r = 0; r < NUM_RACES; r++)
        {
            char* firstDigit = strpbrk(lineContents, "0123456789");
            if (firstDigit == NULL) return;
            char* lastDigit = firstDigit + strspn(firstDigit, "0123456789");
            int length = lastDigit - firstDigit;
            char number[10] = {0};
            strncpy(number, firstDigit, length);
            races[r].distanceToBeat = atoi(number);
            lineContents = lastDigit;
        }
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
        ParseInput(line, l++);
    }
CalculateTime();

    }

