#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    char* line;
    struct ASMLines* next;
} ASMLines;

/** Delete the entire ASM lines list
 *
 * @param head A pointer to the head of the ASM lines list.
 */
void deleteList_asm(ASMLines* head);

/** Add a new ASM line node with the given line to the ASM lines list
 *
 * @param head A pointer to a pointer to the head of the ASM lines list.
 * @param line A pointer to the line of ASM code to be added.
 */
void addNode_asl(ASMLines** head, char* line);

/** Create a new node for ASM lines with the given line
 *
 * @param line A pointer to the line of ASM code.
 * @return Pointer to the newly created ASM line node.
 */
ASMLines * createNode_asl(char* line);
