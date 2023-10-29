#include "CustomStrtok.h"

char *custom_strtok(char *str, const char *delimiters)
{
    /* Holds the current position within the string */
    static char *current_str = NULL;

    /* Points to the beginning of the current token */
    char *token_start = NULL;

    /* Indicates whether currently inside quotes */
    bool in_quotes;

    /* Initialize or reset current_str if str is provided */
    if (str)
        current_str = str;

    /* Skip delimiters to find the start of the next token */
    while (*current_str && strchr(delimiters, *current_str))
        current_str++;

    /* If no more tokens are found, return NULL */
    if (*current_str == '\0')
        return NULL;

    /* Mark the start of the token */
    token_start = current_str;

    /* Initialize in_quotes to false */
    in_quotes = false;

    /* If the token is quoted, advance current_str to the end of the quoted substring */
    if (*current_str == '"') {
        /* Mark the start of the quoted substring */
        token_start = current_str++;

        while (*current_str && (*current_str != '"' || in_quotes)) {
            if (*current_str == '"')
                in_quotes = !in_quotes; /* Toggle in_quotes when a quote is encountered */

            current_str++;
        }

        /* Skip the closing quote */
        if (*current_str == '"')
            current_str++;
    }
        /* If the token is not quoted, advance current_str to the next delimiter */
    else {
        while (*current_str && !strchr(delimiters, *current_str))
            current_str++;
    }

    /* If a token is found, terminate it and move current_str to the next character */
    if (*current_str) {
        /* Terminate the token */
        *current_str = '\0';

        /* Move to the next character after the delimiter */
        current_str++;
    }

    /* Return a pointer to the start of the token */
    return token_start;
}
