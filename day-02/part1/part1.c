#include <stdio.h>
#include <stdbool.h>

#define RED 12
#define GREEN 13
#define BLUE 14

// function declarations
int is_valid_game(char* line);
int get_id(char* line, int* index);
int get_number(char* line, int* index);
void get_color(char* line, int* index, int* color);

int main() {
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    int total = 0;

    while (fgets(line, sizeof(line), input)) {
        total += is_valid_game(line);
    }

    printf("%d\n", total);    
}

// returns the id of the game if it is valid, otherwise returns 0`
int is_valid_game(char* line) {
    // start at index 5 to skip the "Game " part
    int index = 5;

    int id = get_id(line, &index);

    while (line[index] != '\n') {
        index += 2;

        int number = get_number(line, &index);

        int color = 0;
        get_color(line, &index, &color);

        if (number > color) {
            return 0;
        }
    }
    return id;
}

// gets the game id and increments the index
int get_id(char* line, int* index) {
    int id = 0;
    while (line[*index] != ':') {
        id *= 10;
        id += line[*index] - '0';
        (*index)++;
    }
    return id;
}

// gets the number of cubes and increments the index
int get_number(char* line, int* index) {
    int number = 0;
    while (line[*index] != ' ') {
        number *= 10;
        number += line[*index] - '0';
        (*index)++;
    }
    (*index)++;
    return number;
}

// gets the cubes color and increments the index
void get_color(char* line, int* index, int* color) {
    char c = line[*index];
    if (c == 'r') {
        *color = RED;
        (*index) += 3;
    }
    else if (c == 'g') {
        *color = GREEN;
        (*index) += 5;
    }
    else if (c == 'b') {
        *color = BLUE;
        (*index) += 4;
    }
}
