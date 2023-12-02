#include <stdio.h>

struct minimum_cubes
{
    int red;
    int green;
    int blue;
} typedef minimum_cubes_t;

// function declarations
int GetGamePower(char* line);
void SkipID(char* line, int* index);
int GetNumber(char* line, int* index);
void SetMinimumColor(char* line, int* index, int number, minimum_cubes_t* minimum_cubes);

// returns the power of the game
int GetGamePower(char* line)
{
    minimum_cubes_t minimum_cubes = {0, 0, 0};
    
    // start at index 5 to skip the "Game " part
    int index = 5;

    SkipID(line, &index);

    while (line[index] != '\n')
    {
        index += 2;

        int number = GetNumber(line, &index);

        SetMinimumColor(line, &index, number, &minimum_cubes);
    }

    return minimum_cubes.red * minimum_cubes.green * minimum_cubes.blue;
}

// skips the game id and increments the index
void SkipID(char* line, int* index)
{
    while (line[*index] != ':')
    {
        (*index)++;
    }
}

// gets the number of cubes and increments the index
int GetNumber(char* line, int* index)
{
    int number = 0;
    while (line[*index] != ' ')
    {
        number *= 10;
        number += line[*index] - '0';
        (*index)++;
    }
    (*index)++;
    return number;
}

// sets the minimum color if higher than the current minimum
void SetMinimumColor(char* line, int* index, int number, minimum_cubes_t* minimum_cubes)
{
    char c = line[*index];
    if (c == 'r')
    {
        if (number > minimum_cubes->red)
        {
            minimum_cubes->red = number;
        }
        (*index) += 3;
    }
    else if (c == 'g')
    {
        if (number > minimum_cubes->green)
        {
            minimum_cubes->green = number;
        }
        (*index) += 5;
    }
    else if (c == 'b')
    {
        if (number > minimum_cubes->blue)
        {
            minimum_cubes->blue = number;
        }
        (*index) += 4;
    }
}

int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    int total = 0;

    while (fgets(line, sizeof(line), input))
    {
        total += GetGamePower(line);
    }

    printf("%d\n", total);    
}