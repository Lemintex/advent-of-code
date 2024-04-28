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
  part_range_t x; // the range of the x part
  part_range_t m; // the range of the m part
  part_range_t a; // the range of the a part
  part_range_t s; // the range of the s part
} part_group_t;

int organise_part_range(part_range_t *part_range);
workflow_t *get_workflow(char *name);
void split_part_range(part_range_t *part_range, char ch, int split_val);
void copy_part_range();
long int count_accepted_parts(part_group_t group, char *workflow__name);

workflow_t *workflows;
int workflow_count = 0;
part_group_t *part_groups;
int part_group_count = 0;

// TODO - refactor this to be less ugly
int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];
  bool is_reading_instructions = true;

  // get the number of workflows
  while (fgets(line, sizeof(line), input)) {
    if (line[0] == '\n') {
      break;
    }
    workflow_count++;
  }
  rewind(input);

  // allocate memory for the workflows
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
  part_group_count = 1;
  part_groups = (part_group_t *)malloc(part_group_count * sizeof(part_group_t));
  part_groups[0].x.min = 1;
  part_groups[0].x.max = 4000;
  part_groups[0].m.min = 1;
  part_groups[0].m.max = 4000;
  part_groups[0].a.min = 1;
  part_groups[0].a.max = 4000;
  part_groups[0].s.min = 1;
  part_groups[0].s.max = 4000;

  printf("Total: %ld\n", count_accepted_parts(part_groups[0], "in"));
}

// recursive function to count the number of accepted parts
long int count_accepted_parts(part_group_t group, char *workflow__name) {
  long int count = 0;
  if (strcmp(workflow__name, "R") == 0) {
    return 0;
  }
  if (strcmp(workflow__name, "A") == 0) {
    count = 1;
    count *= group.x.max - group.x.min + 1;
    count *= group.m.max - group.m.min + 1;
    count *= group.a.max - group.a.min + 1;
    count *= group.s.max - group.s.min + 1;
    return count;
  }

  // we need to get the workflow
  workflow_t *workflow = get_workflow(workflow__name);

  // loop through the rules
  for (int i = 0; i < workflow->rule_count; i++) {
    rule_t rule = workflow->rules[i];

    part_group_t T = group;
    part_group_t F = group;

    if (rule.comparator == '<') {
      switch (rule.type) {
      case 'x':
        T.x.min = group.x.min;
        T.x.max = rule.value - 1;
        F.x.min = rule.value;
        F.x.max = group.x.max;
        break;
      case 'm':
        T.m.min = group.m.min;
        T.m.max = rule.value - 1;
        F.m.min = rule.value;
        F.m.max = group.m.max;
        break;
      case 'a':
        T.a.min = group.a.min;
        T.a.max = rule.value - 1;
        F.a.min = rule.value;
        F.a.max = group.a.max;
        break;
      case 's':
        T.s.min = group.s.min;
        T.s.max = rule.value - 1;
        F.s.min = rule.value;
        F.s.max = group.s.max;
        break;
      }
    } else if (rule.comparator == '>') {
      switch (rule.type) {
      case 'x':
        T.x.min = rule.value + 1;
        T.x.max = group.x.max;
        F.x.min = group.x.min;
        F.x.max = rule.value;
        break;
      case 'm':
        T.m.min = rule.value + 1;
        T.m.max = group.m.max;
        F.m.min = group.m.min;
        F.m.max = rule.value;
        break;
      case 'a':
        T.a.min = rule.value + 1;
        T.a.max = group.a.max;
        F.a.min = group.a.min;
        F.a.max = rule.value;
        break;
      case 's':
        T.s.min = rule.value + 1;
        T.s.max = group.s.max;
        F.s.min = group.s.min;
        F.s.max = rule.value;
        break;
      }
    }

    char *next_workflow;
    bool failed_all = false;
    if (rule.type == 'x') {
      if (T.x.min <= T.x.max) {
        count += count_accepted_parts(T, rule.targetWorkflowName);
      }
      if (F.x.min <= F.x.max && !failed_all) {
        group = F;
      } else {
        break;
      }
    } else if (rule.type == 'm') {
      if (T.m.min <= T.m.max) {
        count += count_accepted_parts(T, rule.targetWorkflowName);
      }
      if (F.m.min <= F.m.max && !failed_all) {
        group = F;
      } else {
        break;
      }
    } else if (rule.type == 'a') {
      if (T.a.min <= T.a.max) {
        count += count_accepted_parts(T, rule.targetWorkflowName);
      }
      if (F.a.min <= F.a.max && !failed_all) {
        group = F;
      } else {
        break;
      }
    } else if (rule.type == 's') {
      if (T.s.min <= T.s.max) {
        count += count_accepted_parts(T, rule.targetWorkflowName);
      }
      if (F.s.min <= F.s.max && !failed_all) {
        group = F;
      } else {
        break;
      }
    }
  }
  count += count_accepted_parts(group, workflow->final_rule);
  return count;
}

void split_part_group(part_group_t *part_range, char ch, int split_val) {
  copy_part_range();
}

void copy_part_range() {
  part_groups = (part_group_t *)realloc(
      part_groups, part_group_count * sizeof(part_group_t) + 1);
  part_groups[part_group_count] = part_groups[part_group_count - 1];
  part_group_count++;
}

workflow_t *get_workflow(char *name) {
  for (int i = 0; i < workflow_count; i++) {
    if (strcmp(workflows[i].name, name) == 0) {
      return &workflows[i];
    }
  }
  return NULL;
}
