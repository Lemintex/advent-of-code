#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 256

char** dish;
int dishSize = 0;

void RollDishNorth()
{
    int total = 0;
    char** newDish = malloc(dishSize * sizeof(char*));
    for (int i = 0; i < dishSize; i++) // for each row
    {
        newDish[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
        for (int j = 0; j < dishSize; j++) // for each column
        {
            newDish[i][j] = dish[i][j];
            int k = i;
            bool found = false;
            for (k = i; k > 0 && dish[k - 1][j] == '.' && dish[k][j] == 'O'; k--)
            {
                found = true;
                newDish[k - 1][j] = 'O';
                newDish[k][j] = '.';
            }
            if (found || dish[k][j] == 'O')
            {
                total += dishSize - k;
            }
        }
    }
    printf("Total: %d\n", total);
    for (int i = 0; i < dishSize; i++)
    {
        printf("%s", newDish[i]);
    }
    FILE* output = fopen("../output.txt", "w");
    for (int i = 0; i < dishSize; i++)
    {
        fprintf(output, "%s", newDish[i]);
    }
}

int main()
{
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
        printf("%s", dish[i]);
    }
    RollDishNorth();
}