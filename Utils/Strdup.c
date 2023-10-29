#include "Strdup.h"

char* strdup(const char* str)
{
    int len;
    char* new_str = NULL; /* Pointer to the duplicated string */

    if (str == NULL)
        return NULL;

    len = strlen(str) + 1; /* Calculate the length of the string, including the null terminator */

    /* Allocate memory for the duplicated string */
    new_str = malloc(len * sizeof(char));

    /* Check if memory allocation was successful */
    if (new_str == NULL)
    {
        fprintf(stderr, "Error allocating memory");
        exit(0);
    }

    /* Copy the contents of the input string to the new memory location */
    strcpy(new_str, str);

    return new_str; /* Return the pointer to the duplicated string */
}
