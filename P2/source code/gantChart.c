//
// Created by Khent Nathanael Kurniadhi on 3/3/26.
//
#include <stdio.h>
#include <string.h>
#include "task.h"

const char *month_names[MONTH] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

// Function to clear the terminal
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void loadDefault(task t[], int *taskCount) {
    // First task
    strcpy(t[0].name, "Find_Bank");
    t[0].monthStart = january;
    t[0].monthEnd = february;
    for (int i = 0; i < MAX_DEPENDENCIES; i++) {
        t[0].dependencies[i] = -1;
    }
    t[0].countDependencies = 0;

    // Second task
    strcpy(t[1].name, "Get_mortgage_approval");
    t[1].monthStart = march;
    t[1].monthEnd = march;
    t[1].dependencies[0] = 0;
    for (int i = 1; i < MAX_DEPENDENCIES; i++) {
        t[1].dependencies[i] = -1;
    }
    t[1].countDependencies = 1;

    // Third task
    strcpy(t[2].name, "Draw_down");
    t[2].monthStart = march;
    t[2].monthEnd = september;
    t[2].dependencies[0] = 0;
    t[2].dependencies[1] = 1;
    for (int i = 2; i < MAX_DEPENDENCIES; i++) {
        t[2].dependencies[i] = -1;
    }
    t[2].countDependencies = 2;

    // Fourth task
    strcpy(t[3].name, "Build_foundation");
    t[3].monthStart = march;
    t[3].monthEnd = april;
    t[3].dependencies[0] = 2;
    for (int i = 1; i < MAX_DEPENDENCIES; i++) {
        t[3].dependencies[i] = -1;
    }
    t[3].countDependencies = 1;

    // Fifth task
    strcpy(t[4].name, "Build_walls");
    t[4].monthStart = may;
    t[4].monthEnd = july;
    t[4].dependencies[0] = 3;
    for (int i = 1; i < MAX_DEPENDENCIES; i++) {
        t[4].dependencies[i] = -1;
    }
    t[4].countDependencies = 1;

    // Sixth task
    strcpy(t[5].name, "Roof_and_ceiling");
    t[5].monthStart = august;
    t[5].monthEnd = september;
    t[5].dependencies[0] = 3;
    t[5].dependencies[1] = 4;
    for (int i = 2; i < MAX_DEPENDENCIES; i++) {
        t[5].dependencies[i] = -1;
    }
    t[5].countDependencies = 2;

    // Seventh task
    strcpy(t[6].name, "Plumbing");
    t[6].monthStart = october;
    t[6].monthEnd = october;
    t[6].dependencies[0] = 5;
    for (int i = 1; i < MAX_DEPENDENCIES; i++) {
        t[6].dependencies[i] = -1;
    }
    t[6].countDependencies = 1;

    // Eighth task
    strcpy(t[7].name, "Electrics");
    t[7].monthStart = october;
    t[7].monthEnd = october;
    t[7].dependencies[0] = 6;
    for (int i = 1; i < MAX_DEPENDENCIES; i++) {
        t[7].dependencies[i] = -1;
    }
    t[7].countDependencies = 1;

    // Ninth task
    strcpy(t[8].name, "Inspect_build");
    t[8].monthStart = november;
    t[8].monthEnd = december;
    t[8].dependencies[0] = 4;
    for (int i = 1; i < MAX_DEPENDENCIES; i++) {
        t[8].dependencies[i] = -1;
    }
    t[8].countDependencies = 1;

    // Tenth task
    strcpy(t[9].name, "Snagging");
    t[9].monthStart = november;
    t[9].monthEnd = december;
    t[9].dependencies[0] = 7;
    t[9].dependencies[1] = 8;
    for (int i = 2; i < MAX_DEPENDENCIES; i++) {
        t[9].dependencies[i] = -1;
    }
    t[9].countDependencies = 2;

    *taskCount = 10;
}

void ganttChart(task t[], int taskCount) {
    int i, j, m, dep, depCol;

    // Print the row border
    for (i = 0; i < BORDER; i++) {
        printf("-");
    }
    printf("\n");

    printf("| %-22s |", "Task");
    for (m = january; m <= december; m++) {
        printf(" %-9s |", month_names[m - 1]);
    }
    printf(" Dependencies |\n");

    // Print the row border
    for (i = 0; i < BORDER; i++) {
        printf("-");
    }
    printf("\n");

    for (i = 0; i < taskCount; i++) {
        printf("| %-22s |", t[i].name);

        for (m = january; m <= december; m++) {
            if (m >= t[i].monthStart && m <= t[i].monthEnd) {
                printf(" %-9s |", "XXX");
            } else {
                printf(" %-9s |", "");
            }
        }

        printf(" ");
        if (t[i].countDependencies == 0) {
            printf("-");
        } else {
            for (dep = 0; dep < t[i].countDependencies; dep++) {
                printf("%d", t[i].dependencies[dep]);
                if (dep < t[i].countDependencies - 1) {
                    printf(",");
                }
            }
        }
        if (t[i].countDependencies == 0) {
            depCol = 13;
        }

        else {
            depCol = 14 - t[i].countDependencies - (t[i].countDependencies - 1);
        }

        printf("%*s\n", depCol, "|");

        // Print the row border
        for (j = 0; j < BORDER; j++) {
            printf("-");
        }
        printf("\n");
    }
}