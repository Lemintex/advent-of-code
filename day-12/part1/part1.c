#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256

typedef struct
{
    char* springs;
    int* configuration;
} springs_t;

springs_t* springs;
int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];
    
    int lines = 0;
    while (fgets(line, sizeof(line), input))
    {
        lines++;
        /* code */
    }
    springs = malloc(lines * sizeof(springs_t));
    rewind(input);

    for (int i = 0; i < lines; i++)
    {
        springs[i].springs = malloc(MAX_LINE_LENGTH * sizeof(char));
        springs[i].configuration = malloc(MAX_LINE_LENGTH * sizeof(int));
        char* start = strpbrk(line, "#?.");
        if (start == NULL) break; 
        char* end = start + strspn(start, "#?.");
        int length = end - start;
        strncpy(springs[i].springs, start, length);
        start = end;
        
        for (int j = 0; start != NULL; j++)
        {
            fscanf(input, "%d", &springs[i].configuration[j]);
            input++;
            start = input;
        }
    }
    for (int i = 0; i < lines; i++)
    {
        printf("%s\n", springs[i].springs);
        for (int j = 0; j < 9; j++)
        {
            printf("%d\n", springs[i].configuration[j]);
        }
    }
}