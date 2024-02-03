#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct rule
{
    char type; // x, m, a, s
    char operator; // <, >
    int value;
    char* targetWorkflowName;
} rule_t;
typedef struct workflow
{
    // store up to 4 rules
    char* name;
    rule_t* rules;
    int ruleCount;
} workflow_t;

typedef struct part
{
    int x, m, a, s;
} part_t;

workflow_t* workflows;
part_t* parts;

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
    }
    rewind(input);

    workflows = (workflow_t*)malloc(instructionCount * sizeof(workflow_t));
    parts = (part_t*)malloc(partCount * sizeof(part_t));
    
    readingInstructions = true;
    while (fgets(line, sizeof(line), input))
    {
        if (readingInstructions)
        {
            if (line[0] == '\n')
            {
                readingInstructions = false;
                continue;
            }
            // this is an instruction
            char* lineCopy = strdup(line);

            char* name = strtok(line, "{");

            // get the rules
            int ruleCount = 1;
            for (int i = 0; i < strlen(lineCopy); i++)
            {
                if (lineCopy[i] == ',')
                {
                    ruleCount++;
                }
            }

                        // get each rule
            rule_t rules[ruleCount];
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
            workflow_t workflow;
            workflow.name = (char*)malloc(strlen(name) * sizeof(char));
            workflow.rules = (rule_t*)malloc(ruleCount * sizeof(rule_t));
            workflows[instructionCount++] = workflow;
            workflow.name = name;
            workflow.ruleCount = 0;
        }
        else // this is a part
        {
            char* name = strtok(line, "{");
            printf("Name: %s\n", name);
            char* x = strtok(name, ",");
            printf("X: %s\n", x);
            char* m = strtok(NULL, ",");
            char* a = strtok(NULL, ",");
            char* s = strtok(NULL, ",");
            int intx = 0, intm = 0, inta = 0, ints = 0;
            intx = atoi(&x[2]);
            intm = atoi(&m[2]);
            inta = atoi(&a[2]);
            ints = atoi(&s[2]);
            parts[partCount] = (part_t){intx, intm, inta, ints};
            partCount++;
            printf("X: %d, M: %d, A: %d, S: %d\n", intx, intm, inta, ints);
        }
    }
    for (int i = 0; i < partCount; i++)
    {
        printf("Part %d: X: %d, M: %d, A: %d, S: %d\n", i, parts[i].x, parts[i].m, parts[i].a, parts[i].s);
    }
}