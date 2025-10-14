#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "file_utils.h"
#include "util.h"
#include "tea.h"

#define MARKER "JRNL_V1"

#define MAX_ENTRY_SIZE 4096

#define KEY_LEFT_ARROW 0x08
#define KEY_RETURN 0x0D

static char temp_buffer[MAX_ENTRY_SIZE + 8];

/* --- Private Helper Functions --- */

/*
 * Reads a line of input from the user, with simple editing.
 * buffer: Where to store the input string.
 * buffer_size: The maximum size of the buffer.
 */
static void read_password(char *buffer, unsigned int buffer_size)
{
    char key;
    unsigned int len = 0;

    /* Clear the buffer to start */
    buffer[0] = '\0';

    for (;;)
    {
        key = cgetc();

        if (key == KEY_RETURN)
        {
            /* User is done */
            break;
        }
        else if (key == KEY_LEFT_ARROW)
        {
            /* Handle backspace */
            if (len > 0)
            {
                len--;
                buffer[len] = '\0';
                gotox(wherex() - 1);
                cputc(' ');
                gotox(wherex() - 1);
            }
        }
        else if (key >= ' ' && len < buffer_size - 1)
        {
            /* Handle a printable character */
            cputc('*');
            buffer[len] = key;
            len++;
            buffer[len] = '\0'; /* Keep it null-terminated */
        }
    }
}

/* A simple function to derive a 128-bit (16-byte) key from a password string. */
static void derive_key_from_password(uint32_t *key, const char *password)
{
    unsigned char *key_ptr = (unsigned char *)key;
    unsigned int i;
    unsigned int pass_len = strlen(password);

    /* Copy password bytes into the key, repeating if necessary */
    for (i = 0; i < 16; ++i)
    {
        key_ptr[i] = (pass_len > 0) ? password[i % pass_len] : 0;
    }
}

/* --- Public Functions --- */

const char *get_file_util_error_string(file_util_error_t code)
{
    switch (code)
    {
    case FILE_UTIL_SUCCESS:
        return "Success.";
    case FILE_UTIL_ERR_OPEN_FAILED:
        return "Could not open file.";
    case FILE_UTIL_ERR_DECRYPT_FAILED:
        return "Decryption failed. Wrong password or corrupt file.";
    case FILE_UTIL_ERR_BUFFER_TOO_SMALL:
        return "File is too large to load.";
    case FILE_UTIL_ERR_FILE_TOO_SMALL:
        return "The file format is invalid";
    default:
        return "An unknown error occurred.";
    }
}

void get_filename_from_user(char *filename_buffer, unsigned int buffer_size)
{
    char key;
    unsigned int len;

    memset(filename_buffer, 0, buffer_size);
    len = 0;

    printf("Enter filename: ");

    for (;;)
    {
        key = cgetc();

        if (key == KEY_RETURN)
        {
            if (len > 0)
            {
                break;
            }
            else
            {
                /* don't let user enter an empty filename */
                continue;
            }
        }
        else if (key == KEY_LEFT_ARROW)
        {
            if (len > 0)
            {
                len--;
                filename_buffer[len] = '\0';
                gotox(wherex() - 1);
                cputc(' ');
                gotox(wherex() - 1);
            }
        }
        else if (key >= ' ' && len < buffer_size - 1)
        {
            cputc(key);
            filename_buffer[len] = key;
            len++;
        }
    }
}

file_util_error_t save_encrypted_file(const char *filename, const char *data, unsigned int data_size)
{
    FILE *fp;
    char password[17]; /* Max 16 chars for password + null terminator */
    uint32_t key[4];
    unsigned char block[8];
    unsigned int i;
    unsigned char padding_val;

    unsigned int total_size;

    clrscr();
    printf("File save\n");
    print_horizontal_rule();
    printf("\n");

    /* 1. Get Password */
    printf("Enter password (max 16 chars)\nfor '%s': ", filename);
    read_password(password, 17);

    /* 2. Derive Key */
    derive_key_from_password(key, password);

    strcpy(temp_buffer, MARKER);
    memcpy(temp_buffer + 8, data, data_size);
    total_size = 8 + data_size;
    /* 3. Open File */
    fp = (FILE *)fopen(filename, "wb"); /* 'wb' is Write Binary */
    if (!fp)
    {
        return FILE_UTIL_ERR_OPEN_FAILED;
    }

    printf("\n\nEncrypting and saving");

    /* 4. Encrypt and Write Full 8-Byte Blocks */
    for (i = 0; i < total_size / 8; ++i)
    {
        memcpy(block, temp_buffer + (i * 8), 8);
        encrypt((uint32_t *)block, key);
        fwrite(block, 1, 8, fp);

        cputc('.');
    }

    /* 5. Handle the Last Block (Padding) */
    /* We use PKCS#7 padding: if the last block has N bytes, we add (8-N) bytes, */
    /* and the value of each added byte is (8-N). */
    padding_val = 8 - (total_size % 8);
    memcpy(block, temp_buffer + (i * 8), total_size % 8);
    memset(block + (total_size % 8), padding_val, padding_val);

    encrypt((uint32_t *)block, key);
    fwrite(block, 1, 8, fp);

    /* 6. Close File */
    fclose(fp);

    printf("Done.\n\n");
    return FILE_UTIL_SUCCESS;
}

file_util_error_t load_encrypted_file(const char *filename, char *output_buffer, unsigned int buffer_size)
{
    FILE *fp;
    char password[17];
    uint32_t key[4];
    unsigned char block[8];
    unsigned int total_bytes_read = 0;
    size_t blocks_read;
    unsigned char padding_val;
    int i;

    clrscr();
    printf("File load\n");
    print_horizontal_rule();
    printf("\n");

    /* 1. Get Password */
    printf("Enter password for '%s': ", filename);
    do
    {
        read_password(password, 17);
    } while (password[0] == '\0');

    /* 2. Derive Key */
    derive_key_from_password(key, password);

    /* 3. Open File */
    fp = (FILE *)fopen(filename, "rb"); /* 'rb' is Read Binary */
    if (!fp)
    {
        return FILE_UTIL_ERR_OPEN_FAILED;
    }

    printf("\n\nReading and decrypting:\n");

    /* 4. Read and Decrypt All Blocks */
    if(fread(block, 1, 8, fp) != 8) {
        fclose(fp);
        return FILE_UTIL_ERR_FILE_TOO_SMALL;
    }

    decrypt((uint32_t *)block, key);
    if (strncmp(block, MARKER, 8) != 0)
    {
        fclose(fp);
        return FILE_UTIL_ERR_DECRYPT_FAILED;
    }

    while ((blocks_read = fread(block, 1, 8, fp)) == 8)
    {
        decrypt((uint32_t *)block, key);

        /* Ensure we don't overflow the output buffer */
        if (total_bytes_read + 8 > buffer_size)
        {
            fclose(fp);
            return FILE_UTIL_ERR_BUFFER_TOO_SMALL;
        }

        memcpy(output_buffer + total_bytes_read, block, 8);
        total_bytes_read += 8;

        cputc('.');
    }

    fclose(fp);

    if (total_bytes_read == 0)
    {
        /* File only contained a marker, which is a valid empty entry. */
        output_buffer[0] = '\0';
        return FILE_UTIL_SUCCESS;
    }

    /* Handle PKCS#7 padding removal. */
    /* The last byte of the decrypted data contains the padding length. */
    padding_val = output_buffer[total_bytes_read - 1];

    /* The padding value must be between 1 and 8. */
    if (padding_val < 1 || padding_val > 8) {
        return FILE_UTIL_ERR_DECRYPT_FAILED; /* Invalid padding value */
    }

    /* The final content size is the total size minus the padding. */
    total_bytes_read -= padding_val;

    /* Clear out the padding values */
    for (i = total_bytes_read; i < total_bytes_read + padding_val; i++) {
        output_buffer[i] = '\0';
    }

    return FILE_UTIL_SUCCESS;
}