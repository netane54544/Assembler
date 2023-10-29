#ifndef MAMAN14_DECIMALTOBINARY_H
#define MAMAN14_DECIMALTOBINARY_H
#include "Globals.h"
#include <stdbool.h>

/**
 * Convert a decimal number to a binary string representation.
 *
 * @param num The decimal number to be converted.
 * @param binary The character array to store the binary representation.
 * @param length The length of the binary representation.
 * @param startIndex The index at which to start placing the binary digits.
 */
void dec2bin(int num, char *binary, unsigned short length, unsigned int startIndex);

#endif
