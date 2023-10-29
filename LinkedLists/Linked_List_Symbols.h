#ifndef MAMAN14_SYM
#define MAMAN14_SYM
#include "../Token.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    char* label;
    int line;
    unsigned int flag; /* 0 normal, 1 entry, 2 extern */
    struct Simbols_Table* next;
} Simbols_Table;

/** Create a new node for the symbols table with the given label, line, and flag
 *
 * @param label Label for the new node.
 * @param line Line number associated with the label.
 * @param flag Flag value indicating properties of the label.
 * @return Pointer to the newly created node.
 */
Simbols_Table* createNode_s(char* label, int line, unsigned int flag);

/** Add a new node with the given label, line, and flag to the symbols table
 *
 * @param head Pointer to the head of the symbols table.
 * @param label Label of the symbol to be added.
 * @param line Line number associated with the symbol.
 * @param flag Flag value indicating special properties of the symbol.
 */
void addNode_s(Simbols_Table* head, char* label, int line, unsigned int flag);

/** Check if a given label exists in the symbols table
 *
 * @param head Pointer to the head of the symbols table.
 * @param label Label to be checked.
 * @return True if the label exists, otherwise false.
 */
bool isLabel(Simbols_Table* head, char* label);

/** Print the contents of the symbols table
 *
 * @param head Pointer to the head of the symbols table.
 */
void printList_s(Simbols_Table* head);

/** Find a node in the symbols table with a specific label
 *
 * @param head Pointer to the head of the symbols table.
 * @param value Label to be found.
 * @return Pointer to the node with the specified label, or NULL if not found.
 */
Simbols_Table* findInTable_s(Simbols_Table* head, char* value);

/** Delete the entire symbols table
 *
 * @param head Pointer to the head of the symbols table.
 */
void deleteList_s(Simbols_Table* head);

/** Delete the last node from the symbols table
 *
 * @param head Pointer to the head of the symbols table.
 */
void deleteLastNode_s(Simbols_Table* head);

/** Check if there is an entry in the symbols table
 *
 * @param head Pointer to the head of the symbols table.
 * @return True if an entry exists, otherwise false.
 */
bool isEntry_s(Simbols_Table* head);

#endif
