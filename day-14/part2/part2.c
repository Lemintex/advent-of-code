#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 256

char** dish;
int dishSize = 0, lineLength = 0;

//working
void RollDishNorth()
{
    int total = 0;
    for (int i = 0; i < dishSize; i++) // for each row
    {
        for (int j = 0; j < dishSize; j++) // for each column
        {
            int k = i;
            bool found = false;
            for (k = i; k > 0 && dish[k - 1][j] == '.' && dish[k][j] == 'O'; k--)
            {
                found = true;
                dish[k - 1][j] = 'O';
                dish[k][j] = '.';
            }
        }
    }
}

// working
void RollDishEast()
{
    int total = 0;
    for (int i = lineLength - 1; i >= 0; i--) // for each column 
    {
        for (int j = 0; j < dishSize; j++) // for each row 
        {
            int k = i;
            bool found = false;
            for (k = i; k < lineLength - 1 && dish[j][k + 1] == '.' && dish[j][k] == 'O'; k++)
            {
                found = true;
                dish[j][k + 1] = 'O';
                dish[j][k] = '.';
            }
        }
    }
}

//working
void RollDishSouth()
{
    int total = 0;
    for (int i = dishSize - 1; i >= 0; i--) // for each row
    {
        for (int j = 0; j < lineLength; j++) // for each column
        {
            int k = i;
            bool found = false;
            for (k = i; k < dishSize - 1 && dish[k + 1][j] == '.' && dish[k][j] == 'O'; k++)
            {
                found = true;
                dish[k + 1][j] = 'O';
                dish[k][j] = '.';
            }
        }
    }
}

void RollDishWest()
{
    int total = 0;
    for (int i = 0; i < lineLength; i++) // for each row
    {
        for (int j = 0; j < dishSize; j++) // for each column
        {
            int k = i;
            bool found = false;
            for (k = i; k > 0 && dish[j][k - 1] == '.' && dish[j][k] == 'O'; k--)
            {
                found = true;
                dish[j][k - 1] = 'O';
                dish[j][k] = '.';
            }
        }
    }
}

void RollDish()
{
    RollDishNorth();
    RollDishWest();
    RollDishSouth();
    RollDishEast();
}

int CalculateLoad()
{
    int total = 0;
    for (int i = 0; i < dishSize; i++)
    {
        for (int j = 0; j < lineLength; j++)
        {
            if (dish[i][j] == 'O')
            {
                total += dishSize - i;
            }
        }
    }
    return total;
}   
int main()
{
    int total = 0;
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    while (fgets(line, sizeof(line), input))
    {
        dishSize++;
    }

    dish = malloc(dishSize * sizeof(char*));

    rewind(input);
    for (int i = 0; i < dishSize; i++)
    {
        dish[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
        fgets(dish[i], MAX_LINE_LENGTH, input);
    }
    lineLength = strlen(dish[0]);

    int loopCount = 0;
    for (int i = 0; i < 1000; i++)
    {
        RollDish();
    }
    int reference = CalculateLoad();
    do
    {
        RollDish();
        loopCount++;
    }
    while (CalculateLoad() != reference);

    int mod = (1000000000 - 1000) % loopCount;

    for (int i = 0; i < mod; ++i)
    {
        RollDish();
    }
    printf("Total: %d\n", CalculateLoad());

    for (int i = 0; i < dishSize; i++)
    {
        printf("%s", dish[i]);
        free(dish[i]);
    }
}