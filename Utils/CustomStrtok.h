#ifndef MAMAN14_CUSTOMSTRTOK_H
#define MAMAN14_CUSTOMSTRTOK_H

#include <stdbool.h>
#include <string.h>

/**
 * A custom version of strtok that considers quoted substrings as tokens.
 *
 * @param str The string to tokenize (or NULL to continue from previous string).
 * @param delimiters A string containing delimiter characters.
 * @return A pointer to the next token, or NULL if no more tokens are found.
 */
char* custom_strtok(char *str, const char *delimiters);

#endif
