#include "../LinkedLists/Linked_List_ParseBit.h"
#include "../LinkedLists/Linked_List_Token.h"
#include "../LinkedLists/Linked_List_Symbols.h"

/**
 * Save the encoded binary code to a file as base 64.
 *
 * @param bits The object code bits to be saved.
 * @param fileName The name of the output file.
 * @param dataCounter Pointer to the data counter value.
 * @param imageCounter Pointer to the image counter value.
 */
void saveObject(NodeParse* bits, char* fileName, size_t* dataCounter, size_t* imageCounter);
/**
 * Save the external symbols to a file.
 *
 * @param extTable The table of external symbols to be saved.
 * @param fileName The name of the output file.
 */
void saveExt(Token_Node* extTable, char* fileName);
/**
 * Save the entry symbols to a file.
 *
 * @param table The symbol table containing entry symbols.
 * @param fileName The name of the output file.
 */
void saveEnt(Simbols_Table* table, char* fileName);

