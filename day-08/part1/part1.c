#include <stdio.h>

typedef struct
{
    char name[3];
    char l[3];
    char r[3];
} node_t;

node_t nodes[750];
int nodeCount = 750;

char instructions[278];

node_t* FindNode(char* name)
{
    for (int i = 0; i < nodeCount; i++)
    {
        if (nodes[i].name[0] == name[0] && nodes[i].name[1] == name[1] && nodes[i].name[2] == name[2])
        {
            return &nodes[i];
        }
    }
    return NULL;
}

void FollowPath()
{
    int total = 0;
    int pathIndex = 0;
    node_t* current = &nodes[0];
    while(current->name[0] != 'Z' || current->name[1] != 'Z' || current->name[2] != 'Z')
    {
        if (instructions[pathIndex] == 'L')
        {
            current = FindNode(current->l);
        }
        else if (instructions[pathIndex] == 'R')
        {
            current = FindNode(current->r);
        }
        pathIndex++;
        if (pathIndex > 276)
        {
            pathIndex = 0;
        }
        total++;
    }
    printf("Total: %d\n", total);
}
void ParseNode(char* line, int index)
{
    nodes[index].name[0] = line[0];
    nodes[index].name[1] = line[1];
    nodes[index].name[2] = line[2];

    nodes[index].l[0] = line[7];
    nodes[index].l[1] = line[8];
    nodes[index].l[2] = line[9];

    nodes[index].r[0] = line[12];
    nodes[index].r[1] = line[13];
    nodes[index].r[2] = line[14];
}

void ParseInstructions(char* line)
{
    int i = 0;
    int j = 0;
    while (line[i] != '\0')
    {
        if (line[i] == ' ')
        {
            i++;
            continue;
        }
        instructions[j] = line[i];
        i++;
        j++;
    }
}
int main()
{
    int nodeIndex = 0;
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[512];
    int i = 0;
    while (fgets(line, sizeof(line), input))
    {
        if (i == 0)
        {
            ParseInstructions(line);
        }
        else if (i > 1)
        {
            ParseNode(line, nodeIndex);
            nodeIndex++;
        }
        i++;
    } 
    FollowPath();
}