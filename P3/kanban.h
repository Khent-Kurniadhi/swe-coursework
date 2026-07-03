//
// Created by Khent Nathanael Kurniadhi on 2/4/26.
//
#ifndef KANBAN_H
#define KANBAN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAX_SIZE 100

typedef struct item {
    char name[MAX_SIZE];
    struct item *prev;
    struct item *next;
}item;

typedef struct list{
    char name[MAX_SIZE];
    struct list *prev;
    struct list *next;
    item *head;
}list;

extern list *boardHead;

void menu(void);
list *createList(char *owner);
item *createItem(char *name);
void loadFile();
void saveFile();
void loadDefault();
void addList(char *name);
list *findList(char *name);
bool is_valid_item(list *owner, char *name);
bool is_valid_list(list *head, char *owner);
void addItem(list *owner, char *name);
void deleteItem(list *owner, char *name);
void deleteList(char *name);
void editItemName(list *owner, char *oldName, char *newName);
void editItem(list *owner);
void editListName(char *oldName, char *newName);
void editList();
void displayBoard(void);
void displayBoard();

#endif //KANBAN_H