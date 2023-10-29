#ifndef MAMAN14_PARSE
#define MAMAN14_PARSE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    char* name;/*name of macro*/
    char* text; /* array to store */
    struct Linked_mcro* next;/* next in line */

} Linked_mcro;

/**
 * @brief Creates a new node for the Linked_macro list with the given word and name.
 *
 * This function allocates memory for a new node, sets its name and word using dynamically allocated
 * buffers (copies of the provided name and word), and initializes its next pointer to NULL.
 *
 * @param word Pointer to the word to be stored in the new node.
 * @param name Pointer to the name of the word to be stored in the new node.
 * @return Linked_macro* Pointer to the newly created node.
 *                      If memory allocation fails, it returns NULL.
 */
Linked_mcro* createNode(const char* word, const char* name);

/**
 * @brief Frees the memory occupied by the Linked_macro list.
 *
 * This function traverses the Linked_macro list starting from the head and frees the memory
 * occupied by each node in the list, including the memory used for the macro names and their values.
 *
 * @param head Pointer to the head of the Linked_macro list to be freed.
 */
void freeList(Linked_mcro* head);
#endif
