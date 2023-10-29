#ifndef MAMAN14_BASE64_H
#define MAMAN14_BASE64_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * Encodes a binary string to Base64.
 *
 * @param binaryString The input binary string to be encoded. It should be a null-terminated string consisting of '0' and '1' characters.
 *                     The length of the string should be a multiple of 6 for proper encoding.
 * @return The encoded Base64 string. The returned string is dynamically allocated and should be freed by the caller.
 **/
char* encodeBase64(const char* binaryString);

#endif
