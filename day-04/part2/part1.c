#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int cardNumbers[211][10] = {0};
int winningNumbers[211][25] = {0};
int numberOfCards[211] = {0};

void ParseLine(char* line, int cardIndex)
{
    int index = 10; // skip the first 10 characters
    for (int i = 0; i < 10; i++)
    {
        char c = line[index];
        if (line[index] >= '0' && line[index] <= '9')
        {
            cardNumbers[cardIndex][i] *= 10;
            cardNumbers[cardIndex][i] += line[index] - '0';
        }
        index++;
        c = line[index];
        if (line[index] >= '0' && line[index] <= '9')
        {
            cardNumbers[cardIndex][i] *= 10;
            cardNumbers[cardIndex][i] += line[index] - '0';
        }
        
        
        index+=2;
    }
    index += 2; // skip the "or"
    char c = line[index];
    for (int i = 0; i < 25; i++)
    {
        c = line[index];
        if (line[index] >= '0' && line[index] <= '9')
        {
            winningNumbers[cardIndex][i] *= 10;
            winningNumbers[cardIndex][i] += line[index] - '0';
        }
        index++;
        c = line[index];
        if (line[index] >= '0' && line[index] <= '9')
        {
            winningNumbers[cardIndex][i] *= 10;
            winningNumbers[cardIndex][i] += line[index] - '0';
        }
        index+=2;
    }
}
int ReturnGameTotalThatWin()
{
    for (int i = 0; i < 211; i++)
    {
        numberOfCards[i] = 1;
    }

    int total = 0;
    for (int i = 0; i < 211; i++)
    {
        int winningPower = 0;
        for (int j = 0; j < 10; j++)
        {
            for (int k = 0; k < 25; k++)
            {
                int n1 = cardNumbers[i][j];
                int n2 = winningNumbers[i][k];

                if (cardNumbers[i][j] == winningNumbers[i][k])
                {
                    winningPower++;
                    break;
                }
            }
        }
        //PART 1 DONE

        //PART 2
        
        for (int w = 1; w <= winningPower; w++)
        {
            if (i+w >= 211) break;
            numberOfCards[i+w] += numberOfCards[i];
        }
        total += numberOfCards[i];
    }
    printf("total: %d\n", total);
}

int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];
    int i = 0;

    while (fgets(line, sizeof(line), input))
    {
        ParseLine(line, i);
        i++;
    }
   ReturnGameTotalThatWin(); 
}