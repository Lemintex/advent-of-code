#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rule {
  char type;                // x, m, a, s
  char comparator;          // <, >
  int value;                // the value to compare against
  char *targetWorkflowName; // the name of the workflow to go to next
} rule_t;

typedef struct workflow {
  char *name;       // the name of the workflow
  rule_t *rules;    // the rules of the workflow
  int rule_count;   // the number of rules in the workflow
  char *final_rule; // the final rule name of the workflow
} workflow_t;

typedef struct part_range {
  int min; // the minimum value of the part
  int max; // the maximum value of the part
} part_range_t;

typedef struct part_group {
  part_range_t x;  // the range of the x part
  part_range_t m;  // the range of the m part
  part_range_t a;  // the range of the a part
  part_range_t s;  // the range of the s part
  int part_rating; // the total rating of the part
} part_t;

int organise_part_range(part_range_t *part_range);
workflow_t *get_workflow(char *name);
void split_part_range(part_range_t *part_range, char ch, int split_val);
void copy_part_range();

workflow_t *workflows;
int workflow_count = 0;
part_range_t *part_ranges;
int part_range_count = 0;

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];
  bool is_reading_instructions = true;

  // get the number of workflows and parts
  while (fgets(line, sizeof(line), input)) {
    if (line[0] == '\n') {
      break;
    }
    workflow_count++;
  }
  rewind(input);

  // allocate memory for the workflows and parts
  workflows = (workflow_t *)malloc(workflow_count * sizeof(workflow_t));

  is_reading_instructions = true;
  int instruction_index = 0;
  int part_index = 0;
  while (fgets(line, sizeof(line), input)) {
    if (is_reading_instructions) {
      if (line[0] == '\n') {
        is_reading_instructions = false;
        continue;
      }
      // copy the workflow line
      char *lineCopy = strdup(line);

      // get the workflow name
      char *workflow_name = strtok(lineCopy, "{");

      // get the number of rules in the workflow
      // we use the original line as it is not modified
      int rule_count = 0;
      for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ',') {
          rule_count++;
        }
      }

      // allocate memory for the workflow
      workflow_t workflow;
      workflow.name = (char *)malloc(strlen(workflow_name) * sizeof(char));
      workflow.rules = (rule_t *)malloc(rule_count * sizeof(rule_t));

      // defines a temporary array to hold the rules
      rule_t rules[rule_count];

      // loop through the rules
      for (int i = 0; i < rule_count; i++) {

        // get the rule and split it into type, comparator and value
        char *rule = strtok(NULL, ":");
        workflow.rules[i].type = rule[0];
        workflow.rules[i].comparator = rule[1];
        workflow.rules[i].value = atoi(&rule[2]);

        // get the target workflow
        char *str = strtok(NULL, ",");
        workflow.rules[i].targetWorkflowName =
            strdup(str); // strdup mallocs memory
      }
      workflow.rule_count = rule_count;
      workflow.final_rule = strdup(strtok(NULL, "}"));
      strcpy(workflow.name, workflow_name);

      workflows[instruction_index++] = workflow;
    }
  }

  // part 2
  part_range_count = 1;
  part_ranges = (part_range_t *)malloc(part_range_count * sizeof(part_range_t));
}

void split_part_range(part_range_t *part_range, char ch, int split_val) {
  copy_part_range();
  if (ch == '<') {
    part_ranges[part_range_count - 1].max = split_val - 1;
    part_ranges[part_range_count].min = split_val;
  } else {
    part_ranges[part_range_count - 1].max = split_val;
    part_ranges[part_range_count].min = split_val + 1;
  }
}

void copy_part_range() {
  part_ranges = (part_range_t *)realloc(
      part_ranges, part_range_count * sizeof(part_range_t) + 1);
  part_ranges[part_range_count].min = part_ranges[part_range_count - 1].min;
  part_ranges[part_range_count].max = part_ranges[part_range_count - 1].max;
  part_range_count++;
}

workflow_t *get_workflow(char *name) {
  for (int i = 0; i < workflow_count; i++) {
    if (strcmp(workflows[i].name, name) == 0) {
      return &workflows[i];
    }
  }
  return NULL;
}
