#include "CStrcat.h"

char* alloc_strcat(const char *str1, const char *str2) 
{
    /* Calculate the lengths of the input strings */
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    /* Allocate memory for the concatenated string */
    char *result = (char *)malloc(len1 + len2 + 1); /* +1 for the null terminator */

    if (result == NULL) 
    {
	fprintf(stderr, "Memory allocation failed");
        exit(0);
    }

    /* Copy the content of the first string */
    strcpy(result, str1);

    /* Concatenate the content of the second string */
    strcat(result, str2);

    return result;
}
