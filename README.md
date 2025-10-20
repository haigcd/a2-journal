# Retro Journal

A lightweight journaling application designed for 8-bit retro computers, specifically the Apple II family. It provides a basic full-screen text editor and uses the TEA (Tiny Encryption Algorithm) to keep journal entries private.

## Features

*   **Create & Edit Entries**: A straightforward full-screen editor for writing and modifying text.
*   **Secure Encryption**: Entries are encrypted with a user-provided password using the TEA algorithm.
*   **File Management**: Load existing entries or save new ones.
*   **Retro Feel**: Designed with the limitations and user interface paradigms of classic 8-bit systems in mind.

## Controls

### Main Menu

*   `(L)`: Load an existing entry.
*   `(N)`: Create a new entry.
*   `(Q)`: Quit the application.

### Editor

The editor provides basic text entry and navigation.

*   **Arrow Keys**: Move the cursor (Left, Right, Up, Down).
*   **Printable Characters**: Type text into the entry.
*   **Return**: Moves the cursor to the beginning of the next line.
*   **Ctrl+S**: Save the current entry and exit the editor.

## Building from Source

This project is intended to be built using the cc65 cross-compiler toolchain, which targets the 6502 CPU.

### Dependencies

*   `cc65` toolchain (including `cl65` compiler/linker).

### Compilation

To compile the project, you can use a command similar to the following, which links all the C source files together to create a program file for the Apple II.

```sh
cl65 -t apple2 -o journal.prg journal.c menu.c editor.c file_utils.c tea.c util.c
```

This will produce an executable file named `journal.prg` that can be run on an Apple II or in an emulator.

## File Format

Journal entries are saved in a custom encrypted format.

1.  **Marker**: The file begins with an 8-byte marker: `JRNL_V1`. This is used to identify the file type and verify the password during decryption.
2.  **Encryption**: The marker and the entry data are encrypted in 8-byte blocks using the **TEA (Tiny Encryption Algorithm)**. The 128-bit encryption key is derived directly from the user's password.
3.  **Padding**: The final block of data is padded using the **PKCS#7** scheme to ensure it is a full 8 bytes before encryption.

This simple format provides a reasonable level of privacy for your journal entries.

## Project Structure

*   `journal.c`: The main application entry point and program loop.
*   `menu.c` / `menu.h`: Handles the main menu display and user input.
*   `editor.c` / `editor.h`: Implements the full-screen text editor.
*   `file_utils.c` / `file_utils.h`: Manages file I/O, including encryption, decryption, and user prompts for filenames and passwords.
*   `tea.c` / `tea.h`: A standard implementation of the Tiny Encryption Algorithm.
*   `util.c` / `util.h`: Small utility functions used across the application.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
