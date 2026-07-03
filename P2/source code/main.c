//
// Created by Khent Nathanael Kurniadhi on 1/3/26.
//
#include <stdio.h>
#include <string.h>
#include "task.h"

int main() {
    // Initialise variables
    char start[DECISION];
    task t[MAX_TASK];
    int num_tasks = 0;

    printf("Welcome to the Gantt Generator\n");
    printf("Would you like to use the default example or create your own Gantt chart from scratch? (yes/no): ");
    scanf("%19s", start);

    if (strcmp(start, "yes") == 0) {
        // Load default values for tasks
        loadDefault(t, &num_tasks);
        // Display ganttchart
        ganttChart(t, num_tasks);
        // Prompt user for modification options
        userEdit(t, &num_tasks);
    }

    else {
        // Create personal ganttchart from scratch
        newGanttChart(t, &num_tasks);

        // only call function if there are at least 1 task
        if (num_tasks > 0) {
            // Display ganttchart
            ganttChart(t, num_tasks);
            // Prompt user for modification
            userEdit(t, &num_tasks);
        }
    }

    return 0;
}

// ASCII ART

/*
_________        .__  __  .__              .__  _________   .____________.      .___
\_   ___ \_______|__|/  |_|__| ____ _____  |  | \_   ___ \ /              \   __| _/____
/    \  \/\_  __ \  \   __\  |/ ___\\__  \ |  | /    \  \/ |   0      0   |  / __ |/ __ \
\     \____|  | \/  ||  | |  \  \___ / __ \|  |_\     \___ |     ____     | / /_/ \  ___/
 \______  /|__|  |__||__| |__|\___  >____  /____/\______   |     \__/     | \____ |\___  >
        \/                        \/     \/             \//      /  \      \     \/    \/
                                                         /      /    \      \
                                                        /  __   |    |   __  \
                                                       (__/ |   |    |   | \__)
                                                            |   |    |   |
                                                            |   \____/   |
                                                            |            |
                                                         ___\____________/___
                                                          ~~;/           \;~~
*/