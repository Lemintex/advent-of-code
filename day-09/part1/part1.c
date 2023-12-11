#include <stdio.h>
#include <string.h>

int sequence[200][20];
int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

int n = 0;
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
        for (int i = 0; i < 20; i++)
        {
            printf("%d ", sequence[n][i]);
        }
        printf("\n");
    }
    
}