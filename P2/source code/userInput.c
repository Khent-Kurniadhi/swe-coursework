//
// Created by Khent Nathanael Kurniadhi on 1/3/26.
//
#include <stdio.h>
#include <string.h>
#include "task.h"

void userEdit(task t[], int *taskCount) {
    char change[DECISION];

    while (1) {
        clearScreen();
        ganttChart(t, *taskCount);

        printf("\nType 'edit' to edit, 'test' to run a test, or 'quit' to exit: ");
        scanf("%19s", change);

        if (strcmp(change, "edit") == 0) {
            editTaskByName(t, *taskCount);
        }
        else if (strcmp(change, "test") == 0) {
            runDependencyTest(t, *taskCount);
            printf("Press enter to continue");
            getchar();
            getchar();
        }
        else if (strcmp(change, "quit") == 0) {
            break;
        }
        else {
            printf("Invalid option.\n");
        }
    }
}

void newGanttChart(task t[], int *taskCount) {
    int i, j;

    printf("How many tasks would you like to add? ");
    scanf("%d", taskCount);

    if (*taskCount < 1 || *taskCount > MAX_TASK) {
        printf("Invalid number of tasks. Please enter a value from 1 to %d.\n", MAX_TASK);
        *taskCount = 0;
        return;
    }

    for (i = 0; i < *taskCount; i++) {
        printf("\nPlease enter the name of task %d: ", i + 1);
        scanf("%29s", t[i].name);

        printf("Start month (1-12): ");
        scanf("%d", (int *)&t[i].monthStart);

        printf("End month (1-12): ");
        scanf("%d", (int *)&t[i].monthEnd);

        if (t[i].monthStart < january || t[i].monthStart > december ||
            t[i].monthEnd < january || t[i].monthEnd > december ||
            t[i].monthStart > t[i].monthEnd) {
            printf("Invalid month range.\n");
            *taskCount = i;
            return;
        }

        printf("How many dependencies does this task have? ");
        scanf("%d", &t[i].countDependencies);

        if (t[i].countDependencies < 0 || t[i].countDependencies > MAX_DEPENDENCIES) {
            printf("Invalid number of dependencies.\n");
            *taskCount = i;
            return;
        }

        for (j = 0; j < t[i].countDependencies; j++) {
            printf("Enter dependency %d (task index): ", j + 1);
            scanf("%d", &t[i].dependencies[j]);

            if (t[i].dependencies[j] < 0 || t[i].dependencies[j] >= *taskCount) {
                printf("Invalid dependency index.\n");
                *taskCount = i;
                return;
            }
        }

        for (j = t[i].countDependencies; j < MAX_DEPENDENCIES; j++) {
            t[i].dependencies[j] = -1;
        }
    }
}

int findTaskByName(task t[], int taskCount, char name[]) {
    int i;

    for (i = 0; i < taskCount; i++) {
        if (strcmp(t[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

void editTaskByName(task t[], int taskCount) {
    char name[MAX_LENGTH_NAME];
    int index;
    int option;
    int i;

    // Get the name of the task to edit
    printf("Enter the name of the task you want to edit: ");
    scanf("%29s", name);

    // Get task array index
    index = findTaskByName(t, taskCount, name);

    if (index == -1) {
        printf("Task not found.\n");
        return;
    }

    // Ask user which part of task to edit
    printf("\nTask found: %s\n", t[index].name);
    printf("1. Edit task name\n");
    printf("2. Edit start month\n");
    printf("3. Edit end month\n");
    printf("4. Edit dependencies\n");
    printf("Choose an option: ");
    scanf("%d", &option);

    // Edit task name
    if (option == 1){
        printf("Enter a new task name:  ");

        while (getchar() != '\n');
        fgets(t[index].name, MAX_LENGTH_NAME, stdin);
        t[index].name[strcspn(t[index].name, "\n")] = '\0';

        if (strcmp(t[index].name, "") == 0) {
            printf("Invalid task name.\n");
            return;
        }
    }

    // Edit start month
    else if (option == 2) {
        printf("Enter new start month (1-12): ");
        scanf("%d", (int *)&t[index].monthStart);

        if (t[index].monthStart < january || t[index].monthStart > december ||
            t[index].monthStart > t[index].monthEnd) {
            printf("Invalid start month.\n");
            return;
        }

        printf("Start month updated.\n");
    }

    // Edit end month
    else if (option == 3) {
        printf("Enter new end month (1-12): ");
        scanf("%d", (int *)&t[index].monthEnd);

        if (t[index].monthEnd < january || t[index].monthEnd > december ||
            t[index].monthEnd < t[index].monthStart) {
            printf("Invalid end month.\n");
            return;
        }

        printf("End month updated.\n");
    }

    // Edit Dependencies
    else if (option == 4) {
        printf("How many dependencies should this task have? ");
        scanf("%d", &t[index].countDependencies);

        if (t[index].countDependencies < 0 || t[index].countDependencies > MAX_DEPENDENCIES) {
            printf("Invalid number of dependencies.\n");
            return;
        }

        for (i = 0; i < t[index].countDependencies; i++) {
            printf("Enter dependency %d (task index): ", i + 1);
            scanf("%d", &t[index].dependencies[i]);

            if (t[index].dependencies[i] < 0 || t[index].dependencies[i] >= taskCount) {
                printf("Invalid dependency index.\n");
                return;
            }
        }

        for (i = t[index].countDependencies; i < MAX_DEPENDENCIES; i++) {
            t[index].dependencies[i] = -1;
        }

        printf("Dependencies updated.\n");
    }
    else {
        printf("Invalid option.\n");
    }
}

int checkDependency(task t[], int taskIndex, int visited[], int stack[]) {

    // Circular dependencies flag
    if (stack[taskIndex] == 1) {
        printf("%d -> ", taskIndex);
        return 1;
    }

    // skip check if it had already been checked
    if (visited[taskIndex] == 1) {
        return 0;
    }

    // Task path have been checked
    visited[taskIndex] = 1;
    // Task already in current path being checked
    stack[taskIndex] = 1;
    printf("%d -> ", taskIndex);

    for (int i = 0; i < t[taskIndex].countDependencies; i++) {
        int dep = t[taskIndex].dependencies[i];

        if (dep >= 0) {
            if (checkDependency(t, dep, visited, stack)) {
                return 1;
            }
        }
    }

    // reset task to be reused in new path
    stack[taskIndex] = 0;
    return 0;
}

void runDependencyTest(task t[], int taskCount) {
    // Set all task visited and stack flag to 0
    int visited[MAX_TASK] = {0};
    int stack[MAX_TASK] = {0};

    for (int i = 0; i < taskCount; i++) {
        if (checkDependency(t, i, visited, stack)) {
            printf("Circular dependency detected.\n");
            return;
        }
    }

    printf("No circular dependency detected.\n");
}

void checkSpecificTaskDependency(task t[], int targetTaskIndex) {
    // 1. Initialize the tracking arrays to 0
    int visited[MAX_TASK] = {0};
    int stack[MAX_TASK] = {0};

    printf("Tracing path for Task %d: ", targetTaskIndex);

    // 2. Call your existing recursive function on the target task
    if (checkDependency(t, targetTaskIndex, visited, stack)) {
        printf("\nResult: Circular dependency detected involving Task %d!\n", targetTaskIndex);
    } else {
        printf("\nResult: Task %d's dependency chain is clean (No circular dependencies).\n", targetTaskIndex);
    }
}