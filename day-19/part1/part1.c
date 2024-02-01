#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct rule
{
    char* workflowName;
    char type; // x, m, a, s
    char operator; // <, >
    int value;
    char* targetWorkflowName;
} rule_t;
typedef struct workflow
{
    // store up to 4 rules
    char* name;
    rule_t rules[4];
    int ruleCount;
} workflow_t;


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
        char* lineCopy = strdup(line);
        if (readingInstructions)
        {
            if (line[0] == '\n')
            {
                readingInstructions = false;
                continue;
            }
            // read instruction
            instructionCount++;
            char* name = strtok(line, "{");
            workflow_t workflow;
            workflow.name = name;
            workflow.ruleCount = 0;

            int ruleCount = 1;
            for (int i = 0; i < strlen(lineCopy); i++)
            {
                if (lineCopy[i] == ',')
                {
                    ruleCount++;
                }
            }
            // get each rule
            for (int i = 0; i < ruleCount - 1; i++)
            {
                char* rule = strtok(NULL, ","); 
                printf("Rule: %s\n", rule);
                char instructionType = rule[0];
                printf("Instruction type: %c\n", instructionType);
                char operator = rule[1];
                printf("Operator: %c\n", operator);
                int value = atoi(&rule[2]);
                printf("Value: %d\n", value);
            }
            printf("Rule count: %d\n", ruleCount);
            printf( "Final rule: %s\n", strtok(NULL, "}"));
            printf("%s\n", name);
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