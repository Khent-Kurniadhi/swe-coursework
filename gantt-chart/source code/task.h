//
// Created by Khent Nathanael Kurniadhi on 1/3/26.
//
#ifndef TASK_H
#define TASK_H

#include <stdlib.h>

#define MAX_LENGTH_NAME 30
#define MAX_DEPENDENCIES 10
#define MAX_TASK 10
#define MONTH 12
#define DECISION 20
#define BORDER 185

typedef enum {
    january = 1,
    february,
    march,
    april,
    may,
    june,
    july,
    august,
    september,
    october,
    november,
    december
} month;

typedef struct {
    char name[MAX_LENGTH_NAME];
    month monthStart;
    month monthEnd;
    int dependencies[MAX_DEPENDENCIES];
    int countDependencies;
} task;

extern const char *month_names[MONTH];

void clearScreen();
void loadDefault(task t[], int *taskCount);
void ganttChart(task t[], int taskCount);
void userEdit(task t[], int *taskCount);
void editTaskByName(task t[], int taskCount);
void newGanttChart(task t[], int *taskCount);
int checkDependency(task t[], int taskIndex, int visited[], int stack[]);
void runDependencyTest(task t[], int taskCount);

#endif