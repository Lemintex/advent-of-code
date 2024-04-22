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
bool check_part_rule(int value, rule_t *rule);

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

  for (int i = 0; i < workflow_count; i++) {
    printf("Workflow: %s\n", workflows[i].name);
    for (int j = 0; j < workflows[i].rule_count; j++) {
      printf("Rule: %c %c %d %s\n", workflows[i].rules[j].type,
             workflows[i].rules[j].comparator, workflows[i].rules[j].value,
             workflows[i].rules[j].targetWorkflowName);
    }
    printf("Final rule: %s\n", workflows[i].final_rule);
  }
  // to compute the number we need to iterate through the parts and total the
  // ratings of the accepted ones
  int total = 0;
  for (int i = 0; i < part_range_count; i++) {
    total += organise_part_range(&part_ranges[i]);
  }
  printf("FINAL total: %d\n", total);
}

int organise_part_range(part_range_t *part_range) {
  // always start at the "in" workflow
  workflow_t *current_workflow = get_workflow("in");
  //
  //  while (true) {
  //    bool rule_met = false;
  //    for (int i = 0; i < current_workflow->rule_count; i++) {
  //      rule_t rule = current_workflow->rules[i];
  //      if ((rule.type == 'x' && check_part_rule(part->x, &rule)) ||
  //          (rule.type == 'm' && check_part_rule(part->m, &rule)) ||
  //          (rule.type == 'a' && check_part_rule(part->a, &rule)) ||
  //          (rule.type == 's' && check_part_rule(part->s, &rule))) {
  //        rule_met = true;
  //      }
  //      if (rule_met) {
  //        rule_met = false;
  //        if (strcmp(rule.targetWorkflowName, "A") == 0) {
  //          return part->part_rating;
  //        } else if (strcmp(rule.targetWorkflowName, "R") == 0) {
  //          return 0;
  //        } else {
  //          current_workflow = get_workflow(rule.targetWorkflowName);
  //
  //          // reset the loop index
  //          i = -1;
  //        }
  //      }
  //    }
  //    // if we reach the end of the loop, we have to check the final rule
  //    if (strcmp(current_workflow->final_rule, "A") == 0) {
  //      return part->part_rating;
  //    } else if (strcmp(current_workflow->final_rule, "R") == 0) {
  //      return 0;
  //    } else {
  //      current_workflow = get_workflow(current_workflow->final_rule);
  //    }
  //  }
  return 0;
}

bool check_part_rule(int value, rule_t *rule) {
  if (rule->comparator == '<') {
    return value < rule->value;
  } else {
    return value > rule->value;
  }
}

workflow_t *get_workflow(char *name) {
  for (int i = 0; i < workflow_count; i++) {
    if (strcmp(workflows[i].name, name) == 0) {
      return &workflows[i];
    }
  }
  return NULL;
}
