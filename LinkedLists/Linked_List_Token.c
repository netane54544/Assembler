#include "Linked_List_Token.h"


Token_Node* createNode_t(Token* data)
{
    /* Allocate memory for the new Token_Node */
    Token_Node* node = (Token_Node*)malloc(sizeof(Token_Node));

    if (node == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }

    /* Assign the provided Token data to the node and set next pointer to NULL */
    node->data = data;
    node->next = NULL;
    return node;
}

void addNode_t(Token_Node** head, Token* data)
{
    /* Create a new Token_Node using the provided data */
    Token_Node* newNode = createNode_t(data);
    if (*head == NULL)
    {
        /* If the list is empty, set the new node as the head */
        *head = newNode;
    }
    else
    {
        /* Traverse to the end of the list and append the new node */
        Token_Node* currentNode = *head;
        while (currentNode->next != NULL)
        {
            currentNode = (Token_Node*)currentNode->next;
        }

        currentNode->next = newNode;
    }
}

void printList_t(Token_Node* head)
{
    /* Traverse the list and print each Token's information */
    while (head != NULL)
    {
        printf("Token: %s, Type: %d, Address: %d\n", head->data->value, head->data->tokenType, head->data->lineNumber);
        head = (Token_Node*)head->next;
    }
}

void deleteList_t(Token_Node* head)
{
    /* Traverse the list, free each node's memory, and move to the next node */
    while (head != NULL)
    {
        Token_Node* currentNode = head;
        head = (Token_Node*)head->next;
        free(currentNode);
    }
}
