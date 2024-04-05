#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rule {
  char type;     // x, m, a, s
  char operator; // <, >
  int value;
  char *targetWorkflowName;
} rule_t;

typedef struct workflow {
  // store up to 4 rules
  char *name;
  rule_t *rules;
  int ruleCount;
  bool is_accepted;
} workflow_t;

typedef struct part {
  int x, m, a, s;
  int part_rating;
} part_t;

int organise_part(part_t *part);
workflow_t *get_workflow(char *name);
bool check_part_rule(int value, rule_t *rule);

workflow_t *workflows;
int workflowCount = 0;
part_t *parts;
int partCount = 0;

int main() {
  FILE *input = fopen("../input.txt", "r");

  // big char array to hold the line
  char line[256];
  bool readingInstructions = true;

  while (fgets(line, sizeof(line), input)) {
    if (readingInstructions) {
      if (line[0] == '\n') {
        readingInstructions = false;
        continue;
      }
      workflowCount++;
    } else {
      partCount++;
    }
  }
  rewind(input);

  workflows = (workflow_t *)malloc(workflowCount * sizeof(workflow_t));
  parts = (part_t *)malloc(partCount * sizeof(part_t));

  readingInstructions = true;
  int instruction_index = 0;
  int part_index = 0;
  while (fgets(line, sizeof(line), input)) {
    if (readingInstructions) {
      if (line[0] == '\n') {
        readingInstructions = false;
        continue;
      }
      // this is an instruction
      char *lineCopy = strdup(line);

      char *name = strtok(line, "{");

      // get the rules
      int ruleCount = 1;
      for (int i = 0; i < strlen(lineCopy); i++) {
        if (lineCopy[i] == ',') {
          ruleCount++;
        }
      }

      // get each rule
      rule_t rules[ruleCount];
      for (int i = 0; i < ruleCount - 1; i++) {
        char *rule = strtok(NULL, ",");
        printf("Rule: %s\n", rule);
        char instructionType = rule[0];
        printf("Instruction type: %c\n", instructionType);
        char operator= rule[1];
        printf("Operator: %c\n", operator);
        int value = atoi(&rule[2]);
        printf("Value: %d\n", value);
      }
      printf("Rule count: %d\n", ruleCount);
      printf("Final rule: %s\n", strtok(NULL, "}"));
      printf("%s\n", name);
      workflow_t workflow;
      workflow.name = (char *)malloc(strlen(name) * sizeof(char));
      workflow.rules = (rule_t *)malloc(ruleCount * sizeof(rule_t));
      workflows[instruction_index++] = workflow;
      strcpy(workflow.name, name);
      workflow.ruleCount = 0;
      printf("Instruction count: %d\n", instruction_index);
      printf("Instruction name: %s\n", workflows[instruction_index - 1].name);
    } else {
      char *name = strtok(line, "{");
      printf("Name: %s\n", name);
      char *x = strtok(name, ",");
      printf("X: %s\n", x);
      char *m = strtok(NULL, ",");
      char *a = strtok(NULL, ",");
      char *s = strtok(NULL, ",");
      int intx = 0, intm = 0, inta = 0, ints = 0;
      intx = atoi(&x[2]);
      intm = atoi(&m[2]);
      inta = atoi(&a[2]);
      ints = atoi(&s[2]);
      int part_rating = intx + intm + inta + ints;
      parts[part_index++] = (part_t){intx, intm, inta, ints, part_rating};
      printf("X: %d, M: %d, A: %d, S: %d | T: %d\n", intx, intm, inta, ints,
             part_rating);
    }
  }
  printf("Instruction count: %d\n", workflowCount);
  for (int i = 0; i < instruction_index; i++) {
    printf("Instruction %d: %s\n", i, workflows[i].name);
  }
  printf("Part count: %d\n", part_index);
  for (int i = 0; i < partCount; i++) {
    printf("Part %d: X: %d, M: %d, A: %d, S: %d, total: %d\n", i, parts[i].x,
           parts[i].m, parts[i].a, parts[i].s, parts[i].part_rating);
  }

  // to compute the number we need to iterate through the parts
  workflow_t *first_workflow = get_workflow("in");
  int total = 0;
}

int organise_part(part_t *part) {
  workflow_t *current_workflow = get_workflow("in");
  int total = 0;
  bool part_completed = false;
  while (!part_completed) {
    for (int i = 0; i < current_workflow->ruleCount; i++) {
      rule_t rule = current_workflow->rules[i];
      if (rule.type == 'x') {
        if (check_part_rule(part->x, &rule)) {
          current_workflow = get_workflow(rule.targetWorkflowName);
          break;
        }
      } else if (rule.type == 'm') {
        if (check_part_rule(part->m, &rule)) {
          current_workflow = get_workflow(rule.targetWorkflowName);
          break;
        }
      } else if (rule.type == 'a') {
        if (check_part_rule(part->a, &rule)) {
          current_workflow = get_workflow(rule.targetWorkflowName);
          break;
        }
      } else if (rule.type == 's') {
        if (check_part_rule(part->s, &rule)) {
          current_workflow = get_workflow(rule.targetWorkflowName);
          break;
        }
      }
    }
    if (current_workflow->is_accepted) {
      total += part->part_rating;
    }
  }
  return part->part_rating;
}

bool check_part_rule(int value, rule_t *rule) {
  if (rule->operator== '<') {
    return value < rule->value;
  } else {
    return value > rule->value;
  }
}

workflow_t *get_workflow(char *name) {
  for (int i = 0; i < workflowCount; i++) {
    if (strcmp(workflows[i].name, name) == 0) {
      return &workflows[i];
    }
  }
  return NULL;
}
