#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "menu.h"
#include "editor.h"
#include "file_utils.h"

#define MAX_ENTRY_SIZE 8192
#define MAX_FILENAME_SIZE 16

char entry_buffer[MAX_ENTRY_SIZE];
char filename_buffer[MAX_FILENAME_SIZE];

int main(void)
{
    char choice;
    file_util_error_t result;

    /* This is the main application loop. It will run forever until we quit. */
    for (;;)
    {
        choice = display_menu();
        memset(entry_buffer, 0, MAX_ENTRY_SIZE);

        /* Use a switch statement to act on the user's choice */
        switch (choice)
        {
        case MENU_CHOICE_LOAD:
            get_filename_from_user(filename_buffer, MAX_FILENAME_SIZE);

            result = load_encrypted_file(filename_buffer, entry_buffer, MAX_ENTRY_SIZE);

            if (result != FILE_UTIL_SUCCESS)
            {
                printf("Error: %s\n", get_file_util_error_string(result));
                break;
            }

            // buffer is now loaded, fall through to editor:
        case MENU_CHOICE_NEW:
            run_editor(entry_buffer, MAX_ENTRY_SIZE);

            clrscr();
            get_filename_from_user(filename_buffer, MAX_FILENAME_SIZE);

            printf("Saving to file named '%s'\n", filename_buffer);

            result = save_encrypted_file(filename_buffer, entry_buffer, strlen(entry_buffer));

            if (result == FILE_UTIL_SUCCESS)
            {
                printf("File '%s' saved successfully.\n", filename_buffer);
            }
            else
            {
                printf("Error: %s\n", get_file_util_error_string(result));
            }

            break;

        case MENU_CHOICE_QUIT:
            printf("Goodbye!\n");
            return 0; /* Exit the program */
        }

        printf("\nPress any key to return to the menu...");
        cgetc(); /* Wait for a keypress before looping back */
    }

    return 0; /* Should never be reached */
}