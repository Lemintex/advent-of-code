#include <stdio.h>
#include <string.h>

char universe[140][140 + 1] = {0};

int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    for (int i = 0; fgets(line, sizeof(line), input); i++)
    {
        strcpy(universe[i], line);
        universe[i][strlen(universe[i]) - 1] = '\0';
        /* code */
    }
    printf("%s", universe[0]);
    
}