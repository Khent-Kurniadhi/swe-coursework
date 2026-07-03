#include <stdio.h>
#include "kanban.h"

int main(void) {
    loadDefault();

    menu();

    printf("\n\n");
    printf("      /  \\~~~/  \\\n");
    printf(" ,----(     ..    )\n");
    printf("/      \\__     __/\n");
    printf("\\|         (\\  |(\n");
    printf(" ^\\   /___\\  /\\|\n");
    printf("   |__|   |__|\n");

    return 0;
}

void menu(void)
{
    // store value for user choice
    int choice;


    do
    {

        // Display Menu
        printf(
            "\nMenu\n"
            "1. Display Board\n"
            "2. Load Board From file\n"
            "3. Edit list\n"
            "4. Edit item\n"
            "5. Save board to file\n"
            "6. Quit\n\n");

        // prompt user for choice
        printf("Enter your choice (1-6): ");

        // ensure choice is valid - reprompt if not
        do
        {
            scanf("%d", &choice);
        }
        while (choice > 6 || choice < 1);

        if (choice == 1)
        {
            displayBoard();
        }
        else if (choice == 2)
        {
            loadFile();
        }
        else if (choice == 3)
        {
            editList();
        }
        else if (choice == 4)
        {
            // prompt user for name of owner to edit an item
            char name[MAX_SIZE];

            // prompt user to enter owner name
            printf("Enter owner list name: ");
            scanf("%99s", name);

            list *owner = findList(name);
            if (owner)
            {
                editItem(owner);
            }
        }
        else if (choice == 5)
        {
            saveFile();
        }

    } while (choice != 6);

}
// ASCII ART

/*
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠤⠄⠒⠒⠒⠒⠢⠤⣀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢀⡠⢤⡀⢀⡀⠤⢀⣀⠶⢅⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⢤⠀⠀⠀⠀⠀
⠀⠀⡎⠀⠠⠊⠁⠀⢀⠔⠁⠀⠀⠈⢣⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢳⠀⠀⠀⠀
⠀⡼⠀⢠⠀⠀⠀⡀⢸⠀⠀⠀⠀⠀⠈⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣇⡀⠀⠀
⠸⡀⠀⠄⠀⠀⣶⣦⠀⠳⣄⠀⠀⠀⡄⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡗⡌⢆⠀
⠀⠣⢴⠀⠀⠀⠉⠀⠀⠀⠈⡆⠀⢈⡜⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠘⢨⠀
⠀⠀⢸⠀⠀⢀⡠⡲⠀⢀⡖⠈⠒⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⢠⠘⡄
⠀⠀⠘⡄⣠⡃⠓⠉⠛⢯⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠔⠒⠀⠀⠀⡇⠈⢴⡇
⠀⠀⠀⢣⠀⢧⠀⠀⠀⡆⠉⠉⠆⠀⠀⠑⢶⠒⠂⠹⡀⠳⡤⠤⠀⠀⠹⡄⠈⠀
⠀⠀⠀⠘⡆⣈⡳⠀⠀⡝⠒⠀⢇⡀⠀⠀⢸⠀⠀⠀⣇⣀⢘⠆⠀⠀⠀⢇⠀⠀
⠀⠀⠀⠀⠉⠀⠀⠀⣞⠠⢄⠀⣇⠀⠀⠀⠈⡆⠀⠀⠓⠦⢼⣒⣖⣄⡠⠎⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠙⠢⠯⠥⠤⠔⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
*/