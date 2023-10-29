#include "linked-macro.h"
#include "../Utils/Strdup.h"

#define IS_KEYWORD !(strcmp(name, "add") == 0 || strcmp(name, "sub") == 0 || strcmp(name, "lea") == 0 || strcmp(name, "not") == 0 || strcmp(name, "clr") == 0 || strcmp(name, "inc") == 0 || strcmp(name, "dec") == 0 || strcmp(name, "jmp") == 0 || strcmp(name, "bne") == 0 || strcmp(name, "red") == 0 || strcmp(name, "prn") == 0 || strcmp(name, "jsr") == 0 || strcmp(name, "rts") == 0 || strcmp(name, "stop") == 0 || strcmp(name, "r0") == 0 || strcmp(name, "r1") == 0 || strcmp(name, "r2") == 0 || strcmp(name, "r3") == 0 || strcmp(name, "r4") == 0 || strcmp(name, "r5") == 0 || strcmp(name, "r6") == 0 || strcmp(name, "r7") == 0 || strcmp(name, ".data") == 0||strcmp(name, ".string") == 0|| strcmp(name, ".entry") == 0||strcmp(name, ".extern") == 0||strcmp(name, "mov") == 0)


Linked_mcro* createNode(const char* word, const char* name)
{
    Linked_mcro* newNode = (Linked_mcro*)malloc(sizeof(Linked_mcro));
    if(newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }
    /*Checking if the name is not a preserved assembly keyword*/
    if (IS_KEYWORD)
    {
          /* checking if the macro's name is already preserved */
            /* Allocate memory and copy the name to the new node's name field. */
            newNode->name = strdup(name);

            /* Allocate memory and copy the word to the new node's word field. */
            newNode->text = strdup(word);

            newNode->next = NULL; /* Initialize the next pointer to NULL. */

    }
    else
    {
        fprintf(stderr, "The name \"%s\" is listed as a command name and cannot be delivered.\n", name);
        return NULL;
    }

    return newNode;
}


void freeList(Linked_mcro* head)
{
    Linked_mcro* current = head; /* Initialize a pointer to traverse the list. */

    while (current != NULL)
    {
        Linked_mcro* temp = current; /* Store the current node in a temporary pointer. */
        current = (Linked_mcro*)current->next; /* Move to the next node. */
        free(temp); /* Free the memory used for the node. */
    }
}
