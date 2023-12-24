#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char* label;
    int focal_length;
} lens_t;

typedef struct
{
    lens_t* lens;
    int count;
} box_t;

typedef struct
{
    char* label;
    char* instruction; // can be - or 0-9
} instruction_t;

instruction_t* instructions;
box_t hashmap[256]; // Holiday ASCII String Helper Manual Arrangement Procedure

int RunHash(char* l);

void AddLens(box_t box, char* label, int focal_length)
{
    box.count++;
    box.lens = realloc(box.lens, box.count * sizeof(lens_t));
    box.lens[box.count - 1].label = label;
    box.lens[box.count - 1].focal_length = focal_length;
}

void RemoveLens(box_t box, char* lens)
{
    for (int i = 0; i < box.count; i++)
    {
        if (strcmp(box.lens[i].label, lens))
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
        RemoveLens(hashmap[labelHash], instruction->label);
    }
    else
    {
        // Add lens
        AddLens(hashmap[labelHash], instruction->label, atoi(instruction->instruction));
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

int GetPart2Result()
{
    int result = 0;
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < hashmap[i].count; j++)
        {
            int n = (i+1) *j * hashmap[i].lens[j].focal_length;
            result += n; 
        }
    }
    return result;
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

        if (instruction != NULL) {
            *instruction = '\0';  // Separate label from instruction
            instruction++;
        } else if (focal_length != NULL) {
            *focal_length = '\0';  // Separate label from lens number
            focal_length++;
        } else {
            printf("Invalid input format: %s\n", token);
            continue;
        }

            printf("Label: %s\n", label);
            printf("Instruction: %s\n", instruction ? instruction : "-");

            instruction_t inst = {0};
            inst.label = label;
            inst.instruction = instruction ? instruction : "-";
            ExecuteInstruction(&inst);
            token = strtok(NULL, ",");
        }
    }
   printf("Part 2: %d\n", GetPart2Result()); 
}