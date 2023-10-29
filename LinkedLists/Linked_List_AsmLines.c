#include "Linked_List_AsmLines.h"


ASMLines* createNode_asl(char* line)
{
    ASMLines* node = (ASMLines*)malloc(sizeof(ASMLines));
    if(node == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }

    /* Copy the ASM line into the new node */
    node->line = line;

    /* Set the next pointer to NULL (initially, the node has no next node) */
    node->next = NULL;

    return (ASMLines*)node;
}

void addNode_asl(ASMLines** head, char* line)
{
    ASMLines* newNode = (ASMLines*)createNode_asl(line);
    ASMLines* currentNode = NULL;

    if (*head == NULL)
    {
        /* If the list is empty, the new node becomes the head of the list */
        *head = (ASMLines*)newNode;
    }
    else
    {
        currentNode = (ASMLines*)*head;

        /* Traverse to the last node in the list */
        while (currentNode->next != NULL)
            currentNode = (ASMLines*)currentNode->next;

        /* Append the new node to the end of the list */
        currentNode->next = (ASMLines*)newNode;
    }
}

void deleteList_asm(ASMLines* head)
{
    while (head != NULL)
    {
        ASMLines* currentNode = head;
        head = (ASMLines*)head->next;

        /* Free the memory occupied by the current node */
        free(currentNode);
    }
}
