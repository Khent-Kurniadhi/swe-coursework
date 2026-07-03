//
// Created by Khent Nathanael Kurniadhi on 2/4/26.
//
#include "kanban.h"

list *boardHead = NULL;

list *createList(char *owner){
    list *current = malloc(sizeof(list));
    // set the name of the list owner
    strcpy(current->name, owner);
    // initialise next and previous pointer to NULL
    current->next = NULL;
    current->prev = NULL;
    // initialise the first item pointer to NULL
    current->head = NULL;
    // return the new list pointer
    return current;
}

item *createItem(char *name) {
    // allocate memory for new item
    item *current = malloc(sizeof(item));
    //set the name of item
    strcpy(current->name, name);
    // initialise next and previous pointer to NULL
    current->next = NULL;
    current->prev = NULL;
    // return the new item pointer
    return current;
}


void loadFile() {
    char fileName[100];
    // Ask for file name to open from user
    printf("Enter filename: ");
    scanf("%s", fileName);

    // open the file
    FILE *f = fopen(fileName, "r");
    // return error message if file couldnt be opened
    if (f == NULL) {
        printf("Error: could not open file %s", fileName);
        return;
    }

    char line[200];
    // Loop through the file line by line until fgets return NULL
    while (fgets(line, sizeof(line), f) != NULL) {
        // get the list name using strtok
        // which gets the characters on the line
        // until it hits the first instance of the specified delimiter
        char *listName = strtok(line, ",");

        // only add the list if it doesn't already exist
        if (findList(listName) == NULL) {
            addList(listName);
        }

        // get item name using strtok
        // which gets the characters on the line
        // until it hits the first instance of the specified delimiter
        char *itemName = strtok(NULL, ",");
        // loop through the line to get all the items in the list
        while (itemName != NULL) {
            // link item to the list
            addItem(findList(listName), itemName);
            // get next item
            itemName = strtok(NULL, ",");
        }
    }

    fclose(f);
}

void saveFile() {
    char fileName[100];
    // get user input for the file name
    printf("Enter filename: ");
    scanf("%s", fileName);
    // open file
    FILE *f = fopen(fileName, "w");
    if (f == NULL) {
        printf("Error: could not open file %s", fileName);
        return;
    }

    // initialise list pointer to the head list
    list *currentList = boardHead;
    // loop through all the list until it points to NULL
    while (currentList != NULL) {
        // prints the list name into the file line
        fprintf(f, "%s", currentList->name);

        // initialise item pointer to the first item
        item *currentItem = currentList->head;

        // traverse the items in current list until it points to null
        while (currentItem != NULL) {
            // prints the item names into the file line
            fprintf(f, ",%s", currentItem->name);
            // set the item pointer to the next item
            currentItem = currentItem->next;
        }
        fprintf(f, "\n");
        // set the list pointer to the next list
        currentList = currentList->next;
    }
    printf("Board saved to file");
    // close file
    fclose(f);
}

void loadDefault() {
    // creates and link new lists
    addList("nick");
    addList("Tim");
    addList("Dante");
    addList("Abey");

    // adds items to the default lists
    addItem(findList("nick"), "370 RTX");
    addItem(findList("Tim"), "Oculus Quest 2");
    addItem(findList("Dante"), "Oculus Quest 1");
    addItem(findList("Dante"), "370 RTX");
    addItem(findList("Abey"), "Oculus Pro");
    addItem(findList("Abey"), "Oculus Quest 1");

}

void displayBoard(void) {
    // initialise list pointer to the head list
    list *currentList = boardHead;
    while (currentList != NULL) {
        printf("%s:\n", currentList->name);
        // initialise item pointer to the head item
        item *currentItem = currentList->head;
        while (currentItem != NULL) {
            printf("\t%s\n", currentItem->name);
            // set the item pointer to the next item
            currentItem = currentItem->next;
        }
        // set the list pointer to the next list
        currentList = currentList->next;
    }
}