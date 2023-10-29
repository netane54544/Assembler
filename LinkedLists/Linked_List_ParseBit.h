#ifndef MAMAN14_PARSE
#define MAMAN14_PARSE
#include "../Token.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    char bits[13];
    struct NodeParse* next;
} NodeParse;

/** Create a new node for parsing with the given bits
 *
 * @param bits Array of bits containing the parsed data.
 * @return Pointer to the newly created parsing node.
 */
NodeParse* createNode_p(char bits[13]);

/** Add a new parsing node with the given bits to the parsing list
 *
 * @param head Pointer to a pointer to the head of the parsing list.
 * @param bits Array of bits containing the parsed data to be added.
 */
void addNode_p(NodeParse** head, char bits[13]);

/** Delete the entire list
 *
 * @param head Pointer to the head of the parsing list.
 */
void deleteList_p(NodeParse* head);

#endif
