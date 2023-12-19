#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256
#define MAX_SPRINGS 100
#define MAX_SPRING_LENGTH 10

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

    for (int i = 0; fgets(line, sizeof(line), input); i++)
    {
        char* springList = strtok(line, " ");
        springs[i].springs = (char*)malloc(strlen(springList)* sizeof(char));

        springs[i].springs = strtok(line, " ");

char* n = strtok(NULL, ",");
        for (int j = 0; n != NULL; j++)
        {
            springs[i].configuration = (int*)realloc(springs[i].configuration, (j+1) * sizeof(int));
            springs[i].configuration[j] = atoi(n);
            n = strtok(NULL, ",\n");
            printf("%d, ", springs[i].configuration[j]);
        }
        printf("\n");
    } 
}