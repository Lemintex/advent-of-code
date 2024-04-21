#include <stdio.h>
#include <stdlib.h>

/* Function to compare two files */
int compareFiles(FILE *file1, FILE *file2) {
  char ch1, ch2;
  int line = 1;
  do {
    if (ch1 == '\n' && ch2 == '\n') {
      line++;
    }
    ch1 = fgetc(file1);
    ch2 = fgetc(file2);
    printf("F1: %c F2: %c\n", ch1, ch2);
    if (ch1 != ch2) {
      printf("Line: %d\n", line);
      printf("File 1: %c\n", ch1);
      printf("File 2: %c\n", ch2);
      return -1; // Files are not identical
    }
  } while (ch1 != EOF && ch2 != EOF);

  // If both files have reached end, they are identical
  if (ch1 == EOF && ch2 == EOF) {
    return 0;
  } else {
    return -1; // One file has more content than the other
  }
}

int main() {
  FILE *inputFile = fopen("input.txt", "r");
  FILE *outputFile = fopen("output.txt", "r");

  if (inputFile == NULL || outputFile == NULL) {
    printf("Error: Unable to open one or both files.\n");
    exit(EXIT_FAILURE);
  }

  int result = compareFiles(inputFile, outputFile);
  if (result == 0) {
    printf("The files are identical.\n");
  } else {
    printf("The files are not identical.\n");
  }

  fclose(inputFile);
  fclose(outputFile);

  return 0;
}
