//
// Created by Khent Nathanael Kurniadhi on 9/4/26.
//
#include "kanban.h"

void addList(char *name) {
    //creates a new list
    list *newList = createList(name);
    // check if there is already a list in the board
    if (boardHead == NULL) {
        // sets the current list as the head
        // if it's the first instance of a list
        boardHead = newList;
    }
    else {
        // initialise current pointer to the head list
        list *current = boardHead;
        // finds the last instance of a list
        while (current->next != NULL) {
            // sets current pointer to the next valid list
            current = current->next;
        }
        // links the newest list to the existing list
        current->next = newList;
        newList->prev = current;
    }
}

list *findList(char *name) {
    list *current = boardHead;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// function check to see if an item exists
bool is_valid_item(list *owner, char *name)
{
    item *current = owner->head;

    // traverse the list
    while (current != NULL) {

        // found the item
        if (strcmp(current->name, name) == 0) {

            return true;
        }
        // move to next item if no match found yet
        current = current->next;
    }

    // return false if traversal is complete and no item is found
    return false;
}

bool is_valid_list(list *head, char *owner)
{
    // initialise current pointer to the head list
    list *current = head;

    // traverse the list
    while (current != NULL) {

        // found the item
        if (strcmp(current->name, owner) == 0) {

            return true;
        }
        current = current->next;
    }

    return false;
}

void addItem(list *owner, char *name) {
    // create a new instance of an item
    item *newItem = createItem(name);

    // checks if an item already exist in the item
    if (owner->head == NULL) {
        // set the item as the head item if the list was empty
        owner->head = newItem;
    }
    else {
        // initialise current item pointer
        // to the first item
        item *current = owner->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newItem;
        newItem->prev = current;
    }
}

void deleteItem(list *owner, char *name)
{
    // checks if the list exist and has an item
    if (owner == NULL || owner->head == NULL) {
        return;
    }

    // initialise current pointer to the first item
    item *current = owner->head;

    // traverse the items until it points to  NULL
    while (current != NULL) {
        // checks if current pointer matches item name
        if (strcmp(current->name, name) == 0) {

            // if it's the head node
            if (current->prev == NULL) {
                owner->head = current->next;
                if (current->next != NULL) {
                    current->next->prev = NULL;
                }
            }
            else {
                // links the next pointer of the item before
                // the current one to the next item after the current one
                current->prev->next = current->next;
            }

            // fix next node's prev pointer
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }

            free(current);
            return;
        }
        // move current pointer to the next item
        current = current->next;
    }
}

void deleteList(char *name)
{
    // checks if board contains any list
    if (boardHead == NULL) {
        return;
    }

    // initialise current pointer to the head list
    list *current = boardHead;

    while (current != NULL) {

        if (strcmp(current->name, name) == 0) {

            // Delete all items in this list
            item *it = current->head;
            item *nextItem;

            // loops through all the items and free them
            while (it != NULL) {
                nextItem = it->next;
                free(it);
                it = nextItem;
            }

            // Fix links in the board list

            // if it's the head list
            if (current->prev == NULL) {
                //set the next list as the head list
                boardHead = current->next;
                if (current->next != NULL) {
                    // set the new head list's prev pointe to NULL
                    current->next->prev = NULL;
                }
            } else {
                current->prev->next = current->next;
            }

            // fix next node
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }

            // Free the list node itself
            free(current);
            return;
        }

        current = current->next;
    }
}

// helper function to edit the name of an item
void editItemName(list *owner, char *oldName, char *newName)
{
    // check to see if the head pointer for the list of items exists
    // also check to see if the head points to any list of items
    if (owner == NULL || owner->head == NULL) {
        return;
    }

    // store the first element for traversal
    item *current = owner->head;

    // loop until the end of the list is reached
    while (current != NULL) {

        // check for a match for the name provided
        if (strcmp(current->name, oldName) == 0) {

            // if match is found, replace with new name
            strncpy(current->name, newName, sizeof(current->name) - 1);

            // add null character in correct position
            current->name[sizeof(current->name) - 1] = '\0';

            // end loop once completed
            return;
        }

        // if no match found, move to next item in list
        current = current->next;
    }

}

// function to edit an item
void editItem(list *owner)
{

    // display options to user
    printf(
    "Options\n"
    "1. Edit an item\n"
    "2. Add a new item\n"
    "3. Delete an item\n"
    "4. Return to main menu\n");

    // variable to store user choice
    int choice;

    // prompt user to choose an option
    printf("Enter your option: ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    if (choice == 1)
    {
        // store name of item to edit
        char name[MAX_SIZE];

        // prompt user for name of item to edit
        printf("Enter name of item to edit: ");
        fgets(name, sizeof(name),stdin);
        name[strcspn(name, "\n")] = '\0';

        if (is_valid_item(owner, name))
        {
        // store new name of item
        char new_name[MAX_SIZE];

        // prompt user for the new name of the item
        printf("Enter the new name of the item: ");
            fgets(new_name, sizeof(new_name),stdin);
            new_name[strcspn(new_name, "\n")] = '\0';
        editItemName(owner, name, new_name);

        }
        else
        {
            puts("Invalid item");
            return;
        }
    }
    else if (choice == 2)
    {
        // store name of new item
        char name[MAX_SIZE];

        // prompt user for item name
        printf("Enter item name: ");
        fgets(name, sizeof(name),stdin);
        name[strcspn(name, "\n")] = '\0';

        addItem(owner, name);
    }
    else if (choice == 3)
    {
        // store name of item to delete
        char name[MAX_SIZE];

        // prompt user for item name
        printf("Enter item name: ");
        fgets(name, sizeof(name),stdin);
        name[strcspn(name, "\n")] = '\0';

        if (is_valid_item(owner, name))
        {
            deleteItem(owner, name);
        }
        else
        {
            puts("Invalid item");
            return;
        }
    }
    else
    {
        menu();
    }
}

// helper function to edit the name of a list
void editListName(char *oldName, char *newName)
{
    // check to see if the board is empty
    if (boardHead == NULL) {
        return;
    }

    // store first list on board for traversal
    list *current = boardHead;

    // loop until the end of the list is reached
    while (current != NULL) {

        // check for the name of the owner provided
        if (strcmp(current->name, oldName) == 0) {

            // replace old name with new name
            strncpy(current->name, newName, sizeof(current->name) - 1);

            // add null character in the correct position
            current->name[sizeof(current->name) - 1] = '\0';

            // stop once completed
            return;
        }

        // if no match found move to next item in the list
        current = current->next;
    }

}

// function to edit a list
void editList()
{
    printf(
    "Options\n"
    "1. Edit the name of a list\n"
    "2. Add a new list\n"
    "3. Delete a list\n"
    "4. Return to main menu\n");

    // variable to store user choice
    int choice;

    // prompt user to choose an option
    printf("Enter your option: ");
    scanf("%d", &choice);

    while (getchar() != '\n');

    if (choice == 1)
    {
        // store name of item to edit
        char name[MAX_SIZE];

        // prompt user for name of item to edit
        printf("Enter name of a list to edit: ");
        fgets(name, sizeof(name),stdin);
        name[strcspn(name, "\n")] = '\0';


        if (is_valid_list(boardHead, name))
        {
        // store new name of list
        char new_name[MAX_SIZE];

        // prompt user for the new name of the item
        printf("Enter the new name of the list");
            fgets(new_name, sizeof(new_name),stdin);
            new_name[strcspn(new_name, "\n")] = '\0';

        editListName(name, new_name);

        }
        else
        {
            puts("Invalid list");
            return;
        }
    }
    else if (choice == 2)
    {
        // store name of new item
        char name[MAX_SIZE];

        // prompt user for item name
        printf("Enter list name: ");
        fgets(name, sizeof(name),stdin);
        name[strcspn(name, "\n")] = '\0';

        addList(name);
    }
    else if (choice == 3)
    {
        // store name of item to delete
        char name[MAX_SIZE];

        // prompt user for item name
        printf("Enter list name to delete: ");
        fgets(name, sizeof(name),stdin);
        name[strcspn(name, "\n")] = '\0';

        if (is_valid_list(boardHead, name))
        {
            deleteList(name);
        }
        else
        {
            puts("Invalid list");
            return;
        }
    }
    else
    {
        menu();
    }
}