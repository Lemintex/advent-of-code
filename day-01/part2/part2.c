#include <stdio.h>

#define MAX_NUMBER_LENGTH 6

struct number
{
    char number[MAX_NUMBER_LENGTH];
    int length;
    int num;
} typedef number_t;

int processLine(char* line, number_t* numbers)
{
    int start = 0;
    int end = 0;

    int found = 0;

    for (int i = 0; line[i] != '\0'; i++)
    {
        // check if the current character is a number
        if (line[i] >= '0' && line[i] <= '9')
        {
            if (found == 0)
            {
                start = line[i] - '0';
                found = 1;
            }
            end = line[i] - '0';
            continue;
        }

        // check if the current character is a lowercase letter
        if (line[i] >= 'a' && line[i] <= 'z')
        {
            // check if the current character is the first letter of a number
            for (int j = 0; j < 10; j++)
            {
                int index = i;
                for (int wordIndex = 0; line[index] == numbers[j].number[wordIndex]; wordIndex++, index++)
                {
                    // EOL protection
                    if (line[index] == '\0')
                    {
                        return 1;
                    }

                    if (wordIndex == numbers[j].length - 1)
                    {
                        if (found == 0)
                        {
                            start = numbers[j].num;
                            found = 1;
                        }
                        end = numbers[j].num;
                    }
                }
            }
        }
    }
    return start * 10 + end;
}

int main()
{
    // struct array of numbers in string form, length, and int form
    number_t numbers[] = {
       {"zero", 4, 0},
       {"one", 3, 1},
       {"two", 3, 2},
       {"three", 5, 3},
       {"four", 4, 4},
       {"five", 4, 5},
       {"six", 3, 6},
       {"seven", 5, 7},
       {"eight", 5, 8},
       {"nine", 4, 9}
    };

    FILE* input = fopen("../input.txt", "r");

    int total = 0;

    // big char array to hold the line
	char line[256];

	while (fgets(line, sizeof(line), input))
	{
        total += processLine(line, numbers);
    }
    printf("%d\n", total);
}