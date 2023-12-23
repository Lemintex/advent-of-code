#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int focal_length;
    /* data */
} lens_t;

typedef struct
{
    lens_t* lens;
    int count;
    /* data */
} box_t;

typedef struct
{
    char* label;
    char* instruction; // can be - or 0-9
} instruction_t;

instruction_t* instructions;
box_t hashmap[256]; // Holiday ASCII String Helper Manual Arrangement Procedure

void AddLens(box_t box, int lens)
{
    box.count++;
    box.lens = realloc(box.lens, box.count * sizeof(lens_t));
    box.lens[box.count - 1].focal_length = lens;
}

void RemoveLens(box_t box, int lens)
{
    for (int i = 0; i < box.count; i++)
    {
        if (box.lens[i].focal_length == lens)
        {
            for (int j = i; j < box.count - 1; j++)
            {
                box.lens[j] = box.lens[j + 1];
            }
            box.count--;
            box.lens = realloc(box.lens, box.count * sizeof(lens_t));
            return;
        }
    }
}

void ExecuteInstruction(instruction_t* instruction)
{
    int labelHash = RunHash(instruction->label);

    if (strcmp(instruction->instruction, "-") == 0)
    {
        // Remove lens
        RemoveLens(hashmap[labelHash], atoi(instruction->instruction));
    }
    else
    {
        // Add lens
        AddLens(hashmap[labelHash], atoi(instruction->instruction));
    }
}

int RunHash(char* l)
{
    int hash = 0;
    for (int i = 0; i < strlen(l); i++)
    {
        hash += l[i];
        hash *= 17;
        hash %= 256;
    }
    return hash;
}

int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[200000] = {0};

    int total = 0;
    while (fgets(line, sizeof(line), input))
    {
        char* token = strtok(line, ",");

        char* label, *instruction, *focal_length;
        while (token != NULL)
        {
            label = token;
            instruction = strchr(token, '=');
            focal_length = strchr(token, '-');

            if (instruction != NULL)
            {
                *instruction = '\0';  // Separate label from instruction
                instruction++;
            }
            else if (focal_length != NULL)
            {
                *focal_length = '\0';  // Separate instruction from focal_length
                focal_length++;
            }

            printf("Label: %s\n", label);
            printf("Instruction: %s\n", instruction ? instruction : "-");
            token = strtok(NULL, ",");
        }
        for (int i = 0; i < strlen(line); i++)
        {
            char c = line[i];
        }
        total = RunHash(line);
    }
    printf("Total: %d\n", total);
    
}