#ifndef MAMAN14_TOOL
#define MAMAN14_TOOL
#include "../LinkedLists/linked-macro.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Inserts a space character between consecutive double commas in the input string.
 *
 * This function processes the input string character by character, and if it encounters
 * consecutive double commas (",,"), it inserts a space character between them in the result string.
 *
 * @param str The input string to be processed.
 * @return char* A pointer to the newly allocated result string with spaces inserted.
 *         The caller is responsible for freeing the memory allocated for the result.
 *         If memory allocation fails, it returns NULL.
 */
char* insertSpaceBetweenDoubleCommas(const char* str);

/**
 * @brief Removes comments from a given string.
 *
 * This function removes comments from the input string. It considers comments as starting with a ';' character
 * and ending with a newline character ('\n'). It maintains an inComment flag to keep track of whether it's inside
 * a comment block. Characters within a comment block are skipped while copying non-comment characters to the output.
 *
 * @param str Pointer to the input string containing comments to be removed.
 */
void removeComments(char* str);
/**
 * @brief Removes empty lines (lines containing only whitespace characters) from the input string.
 *
 * This function removes empty lines from the input string by modifying it in place.
 * An empty line is considered to be a line that contains only newline characters ('\n') or
 * horizontal tab characters ('\t') and no other non-whitespace characters.
 *
 * @param str Pointer to the input string that needs to be processed. The resulting string
 *            with empty lines removed will also be stored at this location.
 */
void removeEmptyLines(char* str);

/**
 * @brief Finds and saves macro definitions from the input text.
 *
 * This function searches for macro definitions in the input text and creates Linked_macro nodes
 * to store the macro name and text. It extracts text between "mcro" and "endmcro" tokens,
 * removes it from the input text, and appends it to the node's text. The created nodes are
 * linked in the provided Linked_macro linked list.
 *
 * @param text Pointer to the input text containing macro definitions.
 * @return Linked_mcro* Pointer to the head of the updated Linked_mcro linked list.
 *                      If memory allocation fails, it returns NULL.
 */
Linked_mcro* find_and_save_macros(char *text);

/**
* @brief Inserts a space character between consecutive double commas in the input string.
*
* This function processes the input string character by character, and if it encounters
* consecutive double commas (",,"), it inserts a space character between them in the result string.
*
* @param str The input string to be processed.
* @return char* A pointer to the newly allocated result string with spaces inserted.
*         The caller is responsible for freeing the memory allocated for the result.
*         If memory allocation fails, it returns NULL.
*/
void replaceTargetWithReplacement(char** text, const char* target, const char* replacement);
#endif
