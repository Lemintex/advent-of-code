#include <stdio.h>

int processLine(char* line)
{
    int start = 0;
    int end = 0;

    int found = 0;
    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] >= '0' && line[i] <= '9')
        {
            if (!found)
            {
                start = line[i] - '0';
                found = 1;
            }
            end = line[i] - '0';
        }
    }
    return start * 10 + end;
}

int main()
{
    FILE* input = fopen("../input.txt", "r");
    int sum = 0;
	char line[256];
	while (fgets(line, sizeof(line), input))
	{
        sum += processLine(line);
    }
    printf("%d\n", sum);
}
