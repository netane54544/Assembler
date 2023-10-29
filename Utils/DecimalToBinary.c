#include "DecimalToBinary.h"
#include <stdio.h>

void dec2bin(int num, char *binary, unsigned short length, unsigned int startIndex)
{
    int max_value = 0;
    int i;

    if(startIndex >= length)
    {
        fprintf(stderr,"Error! length can't be the same or lower than the starting index.\n");
        foundError = true;
        return;
    }
    max_value = (1 << (length - 1 - startIndex)) - 1; /* Calculate the maximum decimal value for the given length */

    /* Check if the number is within the range of the specified format */
    if (num > max_value || num < -max_value - 1)
    {
        fprintf(stderr,"Error! Decimal number %d is larger than 12 bits, not enough memory.\n", num);
        foundError = true;
        return;
    }

    /* Convert negative numbers to their two's complement representation */
    if (num < 0)
        num = (1 << length) - ((num < 0) ? -num : num);

    /* Convert decimal number to binary representation starting from the least significant bit */
    for (i = 0; i < length - startIndex; i++)
        binary[(length == 13) ? length - 1 -i - startIndex : i] = ((num >> i) & 1) + '0';

    if(length == 13)
    {
        for (i = startIndex; i < length - startIndex; i++)
            binary[i - 1] = binary[i];
    }

    if(startIndex != 0)
    {
        for(i = 0; i <= startIndex; i++)
            binary[length - 1 - i] = '0';
    }

    binary[(length==13) ? length-1:length] = '\0'; /* Null terminator at the end of the string */
}
