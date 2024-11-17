#include <stdio.h>

typedef struct minimum_cubes
{
    int red;
    int green;
    int blue;
} minimum_cubes_t;

// function declarations
int get_game_power(char* line);
void skip_id(char* line, int* index);
int get_number(char* line, int* index);
void set_minimum_color(char* line, int* index, int number, minimum_cubes_t* minimum_cubes);

// returns the power of the game
int get_game_power(char* line) {
    minimum_cubes_t minimum_cubes = {0, 0, 0};
    
    // start at index 5 to skip the "Game " part
    int index = 5;

    skip_id(line, &index);

    while (line[index] != '\n') {
        index += 2;

        int number = get_number(line, &index);

        set_minimum_color(line, &index, number, &minimum_cubes);
    }

    return minimum_cubes.red * minimum_cubes.green * minimum_cubes.blue;
}

// skips the game id and increments the index
void skip_id(char* line, int* index) {
    while (line[*index] != ':') {
        (*index)++;
    }
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

// sets the minimum color if higher than the current minimum
void set_minimum_color(char* line, int* index, int number, minimum_cubes_t* minimum_cubes) {
    char c = line[*index];
    if (c == 'r') {
        if (number > minimum_cubes->red) {
            minimum_cubes->red = number;
        }
        (*index) += 3;
    }
    else if (c == 'g') {
        if (number > minimum_cubes->green) {
            minimum_cubes->green = number;
        }
        (*index) += 5;
    }
    else if (c == 'b') {
        if (number > minimum_cubes->blue) {
            minimum_cubes->blue = number;
        }
        (*index) += 4;
    }
}

int main() {
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    int total = 0;

    while (fgets(line, sizeof(line), input)) {
        total += get_game_power(line);
    }

    printf("%d\n", total);    
}
