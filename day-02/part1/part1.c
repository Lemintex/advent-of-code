#include <stdio.h>

#define RED 12
#define GREEN 13
#define BLUE 14

// function declarations
int IsValidGame(char* line);
int GetID(char* line, int* index);
int GetNumber(char* line, int* index);
void GetColor(char* line, int* index, int* color);

// returns the id of the game if it is valid, otherwise returns 0`
int IsValidGame(char* line)
{
    // start at index 5 to skip the "Game " part
    int index = 5;

    int id = GetID(line, &index);

    while (line[index] != '\n')
    {
        index += 2;

        int number = GetNumber(line, &index);

        int color = 0;
        GetColor(line, &index, &color);

        if (number > color)
        {
            return 0;
        }
    }
    return id;
}

// gets the game id and increments the index
int GetID(char* line, int* index)
{
    int id = 0;
    while (line[*index] != ':')
    {
        id *= 10;
        id += line[*index] - '0';
        (*index)++;
    }
    return id;
}

// gets the number  of cubes and increments the index
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

// gets the cubes color and increments the index
void GetColor(char* line, int* index, int* color)
{
    char c = line[*index];
    if (c == 'r')
    {
        *color = RED;
        (*index) += 3;
    }
    else if (c == 'g')
    {
        *color = GREEN;
        (*index) += 5;
    }
    else if (c == 'b')
    {
        *color = BLUE;
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
        total += IsValidGame(line);
    }

    printf("%d\n", total);    
}