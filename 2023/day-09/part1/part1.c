// thanks guy on reddit for the idea of using a difference table instead of
// recursion
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SENSORS 200
#define SEQUENCE_LEN 21 // +1 for 'result' at the end`

int sequence[200][SEQUENCE_LEN]
            [SEQUENCE_LEN]; // [sensor][measurement][difference]
int total = 0;

int calculate_prediction() {
  int history = 0;
  for (int i = 0; i < SENSORS; i++) // for each sensor
  {
    history += sequence[i][0][SEQUENCE_LEN - 1]; // last reading in row 0
    for (int j = 1; j < SEQUENCE_LEN; j++) // for each measurement in reading
    {
      for (int k = 0; k < SEQUENCE_LEN - j;
           k++) // for each difference in measurement
      {
        sequence[i][j][k] = sequence[i][j - 1][k + 1] - sequence[i][j - 1][k];
      }
      history +=
          sequence[i][j][SEQUENCE_LEN - j - 1]; // last difference in row j
    }
  }
  return history;
}

int main() {
  FILE *input = fopen("../input.txt", "r");

  // this file input reading is so much more elegant omg
  for (int i = 0; i < SENSORS; ++i) {
    for (int j = 0; j < SEQUENCE_LEN; ++j) {
      fscanf(input, "%d",
             &sequence[i][0][j]); // measurements in row 0 of difference table
    }
  }
  fclose(input);

  printf("Total: %d\n", calculate_prediction());
}
