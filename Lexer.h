#include "LinkedLists/Linked_List_Token.h"
#include "LinkedLists/Linked_List_Symbols.h"
#include "Utils/Globals.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

/**
 * Tokenizes the input string and processes tokens to generate a linked list of Token_Node.
 *
 * @param input The input string to be tokenized.
 * @param table The symbols table.
 * @return A linked list of Token_Node containing the processed tokens.
 */

Token_Node* scanner(char* input, Simbols_Table* table);
