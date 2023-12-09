#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HIGH_CARD 0
#define ONE_PAIR 1
#define TWO_PAIR 2
#define THREE_OF_A_KIND 3
#define FULL_HOUSE 4
#define FOUR_OF_A_KIND 5
#define FIVE_OF_A_KIND 6

typedef struct
{
    int hand[5];
    int bid;
    int strength;
    int rank;
} hand_t;

typedef struct
{
    int card;
    int num;
} hand_type_tracker_t;
hand_t hands[1000] = {0};
int handCount = 0;

void ScoreCards()
{
    for (int i = 0; i < 1000; i++)
    {
        hand_t h = hands[i];
        hand_type_tracker_t tracker[5] = {0};

        int numUnique = 0;
        //for each card in the hand
        for (int j = 0; j < 5; j++)
        {
            int unique = 1;
            // for each tracker
            for (int t = 0; t < numUnique; t++)
            {
                if (h.hand[j] == tracker[t].card)
                {
                    tracker[t].num++;
                    unique = 0;
                    break;
                }
            }
            if (unique)
            {
                tracker[numUnique].card = h.hand[j];
                tracker[numUnique].num = 1;
                numUnique++;
            }
            
        }
        int type = 0;
        if (numUnique == 5)
        {
            type = HIGH_CARD;
        }
        else if (numUnique == 4)
        {
            type = ONE_PAIR;
        }
        else if (numUnique == 3)
        {
            if (tracker[0].num == 2 || tracker[1].num == 2 || tracker[2].num == 2)
            {
                type = TWO_PAIR;
            }
            else
            {
                type = THREE_OF_A_KIND;
            }
        }
        else if (numUnique == 2)
        {
            if (tracker[0].num == 3 || tracker[1].num == 3)
            {
                type = FULL_HOUSE;
            }
            else
            {
                type = FOUR_OF_A_KIND;
            }
        }
        else if (numUnique == 1)
        {
            type = FIVE_OF_A_KIND;
        }        
        printf("Type: %d\n", type);
   }
}

int main()
{
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    while (fgets(line, sizeof(line), input))
    {
        int hand[5] = {0};
        for (int i = 0; i < 5; i++)
        {
            if (line[i] == 'A') hand[i] = 14;
            else if (line[i] == 'K') hand[i] = 13;
            else if (line[i] == 'Q') hand[i] = 12;
            else if (line[i] == 'J') hand[i] = 11;
            else if (line[i] == 'T') hand[i] = 10;
            else hand[i] = line[i] - '0';
        }
        char* bid = &line[5];
        char* firstDigit = strpbrk(bid, "0123456789");
        if (firstDigit == NULL) continue;
        char* lastDigit = firstDigit + strspn(firstDigit, "0123456789");
        int length = lastDigit - firstDigit;
        char number[10] = {0};
        strncpy(number, firstDigit, length);
        int b = atoi(number);
        
        memcpy(hands[handCount].hand, hand, sizeof(hand));
        hands[handCount].bid = b;
        handCount++;
        /* code */
    }    
    ScoreCards();
}
