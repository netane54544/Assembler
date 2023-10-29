#include "Linked_List_Symbols.h"

Simbols_Table* createNode_s(char* label, int line, unsigned int flag)
{
    Simbols_Table* node = (Simbols_Table*)malloc(sizeof(Simbols_Table));
    if(node == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }

    node->label = label;
    node->line = line;
    node->next = NULL;
    node->flag = flag;
    return node;
}

void addNode_s(Simbols_Table* head, char* label, int line, unsigned int flag)
{
    Simbols_Table* newNode = createNode_s(label, line, flag);
    Simbols_Table* currentNode = head;

    if(head->line == -10)
    {
        free(newNode);
        /* Initialize an empty table node */
        head->label = label;
        head->line = line;
        head->next = NULL;
        head->flag = flag;
    }
    else
    {
        /* Traverse the list to the last node */
        while (currentNode->next != NULL)
            currentNode = (Simbols_Table*)currentNode->next;

        /* Append the new node to the end of the list */
        currentNode->next = (Simbols_Table*)newNode;
    }
}

void printList_s(Simbols_Table* head)
{
    Simbols_Table* current = head;

    while (current != NULL)
    {
        /* Print symbol information */
        printf("Symbol: %s, Flag: %d, Address: %d\n", current->label, current->flag, current->line);
        current = (Simbols_Table*)current->next;
    }
}

bool isLabel(Simbols_Table* head, char* label)
{
    Simbols_Table* current = head;

    if(current == NULL || current->label == NULL) /*not initialized */
        return false;

    while(current != NULL)
    {
        /* Compare label with current node's label */
        if(strcmp(label, current->label) == 0)
            return true;

        current = (Simbols_Table*)current->next;
    }

    return false;
}

Simbols_Table* findInTable_s(Simbols_Table* head, char* value)
{
    Simbols_Table* current = head;

    if(head == NULL) /*not initialized */
        return NULL;

    while(current != NULL)
    {
        /* Compare value with current node's label */
        if(strcmp(current->label, value) == 0)
            return current;

        current = (Simbols_Table*)current->next;
    }

    return NULL;
}

bool isEntry_s(Simbols_Table* head)
{
    Simbols_Table* current = head;

    while(current != NULL)
    {
        /* Check if the node represents an entry (flag = 1) */
        if(current->line != -1 && current->flag == 1)
            return true;

        current = (Simbols_Table*)current->next;
    }

    return false;
}

void deleteLastNode_s(Simbols_Table* head)
{
    Simbols_Table* current;
    Simbols_Table* previous;

    if (head == NULL || head->line == -10)
        return; /* Empty list, nothing to delete */

    /* If the list contains only one node */
    if (head->next == NULL) {
        /* Reset the single node's data */
        head->flag = 0;
        head->label = NULL;
        head->next = NULL;
        head->line = -10;
        return;
    }

    current = head;
    previous = NULL;

    /* Traverse to the last node */
    while (current->next != NULL)
    {
        previous = current;
        current = (Simbols_Table*)current->next;
    }

    /* Delete the last node and update the previous node's next pointer */
    free(current);
    previous->next = NULL;
}

void deleteList_s(Simbols_Table* head)
{
    while (head != NULL)
    {
        Simbols_Table* currentNode = head;
        head = (Simbols_Table*)head->next;
        free(currentNode);
    }
}
