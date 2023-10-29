#include "Base64.h"
#include <stdio.h>

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* Function to encode a binary string to Base64 */
char* encodeBase64(const char* binaryString)
{
    int binaryLength = strlen(binaryString);     /* Length of the binary string */
    int outputLength = ((binaryLength + 5) / 6) * 4;  /* Length of the output Base64 string */
    char* output = (char*)malloc(outputLength + 1);   /* Output Base64 string */
    char* p = output;                             /* Pointer to current position in the output string */
    int i;
    unsigned int byteValue = 0;                  /* Value of the current 6-bit group */
    int bitCount = 0;                            /* Count of bits processed in the current group */

    if (output == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(0);
    }

    /* Loop through each character in the binary string */
    for (i = 0; i < binaryLength; i++) {
        byteValue <<= 1;
        if (binaryString[i] == '1') {
            byteValue |= 1;
        }

        bitCount++;

        /* When 6 bits are processed, convert the 6-bit group to Base64 character */
        if (bitCount == 6) {
            *p++ = base64_table[byteValue];
            byteValue = 0;
            bitCount = 0;
        }
    }

    /* If there are remaining bits in the last group, process them and add padding if needed */
    if (bitCount > 0)
    {
        byteValue <<= (6 - bitCount);
        *p++ = base64_table[byteValue];
    }

    /* Null-terminate the output string */
    *p = '\0';

    return output;
}