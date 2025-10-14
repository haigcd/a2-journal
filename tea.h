#ifndef TEA_H
#define TEA_H

#include <stdint.h> // For using uint32_t, which is a 32-bit unsigned integer

/*
 * Encrypts a 64-bit block of data using the TEA algorithm.
 * v: pointer to a two-element array of 32-bit unsigned integers (the 64-bit data block).
 * k: pointer to a four-element array of 32-bit unsigned integers (the 128-bit key).
 */
void encrypt(uint32_t* v, const uint32_t* k);

/*
 * Decrypts a 64-bit block of data using the TEA algorithm.
 * v: pointer to a two-element array of 32-bit unsigned integers (the 64-bit data block).
 * k: pointer to a four-element array of 32-bit unsigned integers (the 128-bit key).
 */
void decrypt(uint32_t* v, const uint32_t* k);

#endif // TEA_H