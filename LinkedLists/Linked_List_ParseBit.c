#include "Linked_List_ParseBit.h"

NodeParse* createNode_p(char bits[13])
{
    NodeParse* node = (NodeParse*)malloc(sizeof(NodeParse));
    if(node == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }

    /* Copy the parsed bits into the new node */
    strcpy(node->bits, bits);

    /* Set the next pointer to NULL (initially, the node has no next node) */
    node->next = NULL;
    return node;
}

void addNode_p(NodeParse** head, char bits[13])
{
    NodeParse* newNode = createNode_p(bits);

    if (*head == NULL)
    {
        /* If the list is empty, the new node becomes the head of the list */
        *head = newNode;
    }
    else
    {
        NodeParse* currentNode = *head;

        /* Traverse to the last node in the list */
        while (currentNode->next != NULL)
        {
            currentNode = (NodeParse*)currentNode->next;
        }

        /* Append the new node to the end of the list */
        currentNode->next = (NodeParse*)newNode;
    }
}

void deleteList_p(NodeParse* head)
{
    while (head != NULL)
    {
        NodeParse* currentNode = head;
        head = (NodeParse*)head->next;

        /* Free the memory occupied by the current node */
        free(currentNode);
    }
}
