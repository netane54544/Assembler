#include "LinkedLists/Linked_List_Token.h"
#include "LinkedLists/Linked_List_Symbols.h"
#include "LinkedLists/Linked_List_ParseBit.h"
#include "Utils/Globals.h"
#include <stdbool.h>

/**
 * The function parses the the tokens we got during the lexer step.
 * It checks for the correctness of the each expression syntax, then it encodes each expression into binary.
 * At the end of the function it will write the data converted into base64 into a file.
 * If there any .entry or .extern label it will also write them in a separate file.
 * @param head -our tokens linked list
 * @param table - our symbols tokens linked list
 * @param dataCounter - counter for the data
 * @param imageCounter  - counter for the image
 */
void parseCommand(Token_Node* head, Simbols_Table* table, size_t* dataCounter, size_t* imageCounter);

