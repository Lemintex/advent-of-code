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

typedef struct {
  int hand[5];
  int bid;
  int strength;
  int rank;
} hand_t;

typedef struct {
  int card;
  int num;
} hand_type_tracker_t;

hand_t hands[1000] = {0};
int hand_count = 0;

// compare data for sorting
int compare(const void *a, const void *b) {
  hand_t *hand_a = (hand_t *)a;
  hand_t *hand_b = (hand_t *)b;
  if (hand_a->strength != hand_b->strength) {
    return hand_a->strength - hand_b->strength;
  }

  for (int i = 0; i < 5; i++) {
    if (hand_a->hand[i] != hand_b->hand[i]) {
      return hand_a->hand[i] - hand_b->hand[i];
    }
  }
  return 0;
}

void score_cards() {
  for (int i = 0; i < 1000; i++) {
    hand_t h = hands[i];
    hand_type_tracker_t tracker[5] = {0};

    int num_unique = 0;
    // for each card in the hand
    for (int j = 0; j < 5; j++) {
      int unique = 1;
      // for each tracker
      for (int t = 0; t < num_unique; t++) {
        if (h.hand[j] == tracker[t].card) {
          tracker[t].num++;
          unique = 0;
          break;
        }
      }
      if (unique && h.hand[j] != 1) {
        tracker[num_unique].card = h.hand[j];
        tracker[num_unique].num = 1;
        num_unique++;
      }
    }
    if (num_unique == 0) {
      num_unique = 1;
    }
    if (num_unique == 5) {
      h.strength = HIGH_CARD;
    } else if (num_unique == 4) {
      h.strength = ONE_PAIR;
    } else if (num_unique == 3) {
      if (tracker[0].num == 2 || tracker[1].num == 2 || tracker[2].num == 2) {
        h.strength = TWO_PAIR;
      } else {
        h.strength = THREE_OF_A_KIND;
      }
    } else if (num_unique == 2) {
      if (tracker[0].num == 3 || tracker[1].num == 3) {
        h.strength = FULL_HOUSE;
      } else {
        h.strength = FOUR_OF_A_KIND;
      }
    } else if (num_unique == 1) {
      h.strength = FIVE_OF_A_KIND;
    }
    hands[i] = h;
    printf("%d\n", h.strength);
  }
  qsort(hands, hand_count, sizeof(hand_t), compare);
  int total = 0;
  for (int i = 0; i < hand_count; i++) {
    total += hands[i].bid * (i + 1);
  }
  printf("%d\n", total);
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];

  while (fgets(line, sizeof(line), input)) {
    int hand[5] = {0};
    for (int i = 0; i < 5; i++) {
      if (line[i] == 'A')
        hand[i] = 14;
      else if (line[i] == 'K')
        hand[i] = 13;
      else if (line[i] == 'Q')
        hand[i] = 12;
      else if (line[i] == 'J')
        hand[i] = 1; // jokers are weak... why so serious?
      else if (line[i] == 'T')
        hand[i] = 10;
      else
        hand[i] = line[i] - '0';
    }
    char *bid = &line[5];
    char *first_digit = strpbrk(bid, "0123456789");
    if (first_digit == NULL)
      continue;
    char *last_digit = first_digit + strspn(first_digit, "0123456789");
    int length = last_digit - first_digit;
    char number[10] = {0};
    strncpy(number, first_digit, length);
    int b = atoi(number);

    memcpy(hands[hand_count].hand, hand, sizeof(hand));
    hands[hand_count].bid = b;
    hand_count++;
    /* code */
  }
  score_cards();
}
