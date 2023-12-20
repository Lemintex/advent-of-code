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
    int springGroups;
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
    }
    springs = malloc(lines * sizeof(springs_t));
    rewind(input);

    for (int i = 0; fgets(line, sizeof(line), input); i++)
    {
        springs[i].springs = (char*)malloc(MAX_SPRING_LENGTH * sizeof(char));
        springs[i].configuration = (int*)malloc(MAX_SPRINGS * sizeof(int));

        char* numStr = strtok(line, " ");
        springs[i].springs = numStr;
        printf("%s\n", springs[i].springs);


        int l = 0;
        springs[i].configuration = (int*)malloc(l * sizeof(int));
        char* n = strtok(NULL, ",");

        for (int j = 0; n != NULL; j++)
        {
            int* temp = realloc(springs[i].configuration, (l + 1) * sizeof(int));
            if (temp == NULL) {
                // handle the error by freeing previously allocated memory and returning
                free(springs[i].configuration);
                return 0;
            }
            springs[i].configuration = temp;
            springs[i].configuration[j] = atoi(n);
            n = strtok(NULL, ",\n");
            l++;
            springs[i].springGroups = l;
            printf("%d, ", springs[i].configuration[j]);
        }
        printf("\n");
        springs[i].springGroups = l;
    }
    printf("Done\n");
}