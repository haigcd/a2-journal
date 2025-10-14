#ifndef EDITOR_H
#define EDITOR_H

/*
 * Runs a simple full-screen text editor.
 * 
 * buffer: A pointer to the character array where the typed text will be stored.
 * buffer_size: The maximum number of characters the buffer can hold.
 */
void run_editor(char* buffer, unsigned int buffer_size);

#endif /* EDITOR_H */