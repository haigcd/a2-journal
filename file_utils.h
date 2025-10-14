#ifndef FILE_UTILS_H
#define FILE_UTILS_H

/* Defines the possible return codes for file utility functions. */
typedef enum {
    FILE_UTIL_SUCCESS = 0,
    FILE_UTIL_ERR_OPEN_FAILED = -1,
    FILE_UTIL_ERR_DECRYPT_FAILED = -2,
    FILE_UTIL_ERR_BUFFER_TOO_SMALL = -3,
    FILE_UTIL_ERR_FILE_TOO_SMALL = -4
} file_util_error_t;


/*
 * Prompts the user for a filename, allowing them to edit a default name.
 *
 * filename_buffer: Pointer to the buffer where the final filename will be stored.
 * buffer_size: The size of the filename_buffer.
 */
void get_filename_from_user(char* filename_buffer, unsigned int buffer_size);

/*
 * Encrypts and saves a buffer of data to a file on disk.
 *
 * filename: The name of the file to save.
 * data: A pointer to the data to be encrypted and saved.
 * data_size: The number of bytes in the data buffer.
 *
 * Returns 0 on success, -1 on failure.
 */
file_util_error_t save_encrypted_file(const char* filename, const char* data, unsigned int data_size);

/*
 * Reads, decrypts, and displays an encrypted file.
 *
 * filename: The name of the file to read.
 * output_buffer: A buffer to store the decrypted content.
 * buffer_size: The maximum size of the output buffer.
 *
 * Returns 0 on success, -1 on failure (file not found, etc.).
 */
file_util_error_t load_encrypted_file(const char* filename, char* output_buffer, unsigned int buffer_size);

/*
 * Converts a file_util_error_t code into a human-readable string.
 *
 * code: The error code to convert.
 *
 * Returns a pointer to a constant string describing the error.
 */
const char* get_file_util_error_string(file_util_error_t code);

#endif /* FILE_UTILS_H */