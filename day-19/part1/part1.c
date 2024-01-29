#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];
    int instructionCount = 0;
    int partCount = 0;
    bool readingInstructions = true;

    while (fgets(line, sizeof(line), input))
    {
        if (readingInstructions)
        {
            if (line[0] == '\n')
            {
                readingInstructions = false;
                continue;
            }
            instructionCount++;
        }
        else
        {
            partCount++;
        }
        // read instruction

        // read machine part
    }
    printf("Instruction count: %d\n", instructionCount);
    printf("Part count: %d\n", partCount);    
}