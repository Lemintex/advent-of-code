#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SEQUENCE_LEN 20

int sequence[200][SEQUENCE_LEN];
int total = 0;

int CalculatePrediction(int n, int* dif)
{

    int* differences = (int*)malloc(sizeof(int) * (n+1)); // +1 for 'result' at the end
    printf("%d: ", n);
    bool isAllZero = true;
    for (int i = 0; i < n-1; i++)
    {
        differences[i] = dif[i+1] - dif[i];
        if (dif[i] != 0)
        {
            isAllZero = false;
        }
        printf("%d ", dif[i]);
    }
    if (isAllZero) 
    {
        free(differences);
        return 0;
    }
    printf("\n");
    int r = 0;
    if (n > 0)
    {
        r = CalculatePrediction(n - 1, differences) + dif[n-1];
    }
    free(differences);
    return r;
}

int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    for (int n = 0; fgets(line, sizeof(line), input); n++)
    {
        int index = 0;
        int currentNumber = 0;
        int sign = 1;
        for (int i = 0; i < strlen(line); i++)
        {
            char c = line[i];
            if (c == ' ' || c == '\n')
            {
                currentNumber *= sign;
                sequence[n][index] = currentNumber;
                currentNumber = 0;
                index++;
                sign = 1;
            }
            else if (c == '-')
            {
                sign = -1;
            }
            else
            {
                currentNumber = currentNumber * 10 + (c - '0');
            }
        }
        /* code */
        //for (int i = 0; i < SEQUENCE_LEN; i++)
        //{
        //    printf("%d ", sequence[n][i]);
        //}
        //printf("\n");
    }
    
    for (int i = 0; i < 200; i++)
    {
        total += CalculatePrediction(SEQUENCE_LEN, sequence[i]);
        
    }
    printf("Total: %d\n", total);
}
// 1025603924 is too low
//2098502699 is too high