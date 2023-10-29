#ifndef MAMAN14_TOKEN_HL
#define MAMAN14_TOKEN_HL
#include "../Token.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    Token* data;
    struct Token_Node* next;
} Token_Node;

/**
 * Create a new Token_Node with the provided Token data.
 *
 * @param data The Token data to store in the new node.
 * @return A pointer to the newly created Token_Node.
 */
Token_Node* createNode_t(Token* data);

/**
 * Add a new node with the provided Token data to the end of the Token_Node list.
 *
 * @param head A pointer to the head of the Token_Node list.
 * @param data The Token data to add to the list.
 */
void addNode_t(Token_Node** head, Token* data);

/**
 * Print the Token_Node list
 *
 * @param head The head of the Token_Node list to be printed.
 */
void printList_t(Token_Node* head);

/**
 * Delete the entire Token_Node list and free the associated memory.
 *
 * @param head The head of the Token_Node list to be deleted.
 */
void deleteList_t(Token_Node* head);

#endif 
