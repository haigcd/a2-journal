#ifndef MENU_H
#define MENU_H

#define MENU_CHOICE_LOAD 'L'
#define MENU_CHOICE_NEW  'N'
#define MENU_CHOICE_QUIT 'Q'

/*
 * Displays the main menu and waits for the user to enter a valid choice.
 * Clears the screen before displaying the menu.
 * Returns the character for the chosen option (e.g., 'R', 'N', or 'Q').
 */
char display_menu(void);

#endif /* MENU_H */