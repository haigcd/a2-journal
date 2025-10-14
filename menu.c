#include <stdio.h>
#include <conio.h>  
#include <ctype.h>  
#include "menu.h"  
#include "util.h"

char display_menu(void) {
    char choice;

    do {
        /* Clear the screen for a clean menu display */
        clrscr();

        /* Print the menu title and options */
        printf("Retro Journal: Menu\n");
        print_horizontal_rule();
        printf("\n");

        printf("Select an option:\n");
        printf("(L)oad an entry\n");
        printf("(N)ew entry\n");
        printf("(Q)uit\n\n");

        printf("Your choice: ");

        /* 
         * cgetc() waits for a single keypress and returns it immediately
         * without the user having to press Enter. It's perfect for menus.
         */
        choice = cgetc();

        /* Convert the choice to uppercase to make it case-insensitive */
        choice = toupper(choice);

    /* Loop until the user presses one of the valid keys */
    } while (choice != MENU_CHOICE_LOAD && 
             choice != MENU_CHOICE_NEW && 
             choice != MENU_CHOICE_QUIT);

    /* Add a couple of newlines to separate the menu from what comes next */
    printf("\n\n");

    return choice;
}