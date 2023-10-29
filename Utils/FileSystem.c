#include "FileSystem.h"
#include "../Utils/Base64.h"

void saveObject(NodeParse* bits, char* fileName, size_t* dataCounter, size_t* imageCounter)
{
    FILE* sw; /* File pointer */
    const short length = strlen(fileName) + 4; /* Length of the output file name */
    char* fileStore = NULL; /* Store the modified file name */
    NodeParse* current = bits; /* Pointer to the current node in the object code list */

    fileStore = malloc(length);
    if(fileStore == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }

    /* Modify the output file name */
    strncpy(fileStore, fileName, length);
    fileStore[length-4] = '.';
    fileStore[length-3] = 'o';
    fileStore[length-2] = 'b';
    fileStore[length-1] = '\0';

    /* Open the output file for writing */
    if ((sw = fopen(fileStore, "w")) == NULL) {
        fprintf(stderr, "Error! Couldn't create or override the file %s", fileStore);
        return;
    }
    free(fileStore);    

    /* Write image counter and data counter to the file */
    fprintf(sw, "%d, %d\n", *imageCounter, *dataCounter);

    /* Write encoded object code bits to the file */
    while (current != NULL) {
        fprintf(sw, "%s\n", encodeBase64(current->bits));
        current = (NodeParse*)current->next;
    }

    /* Close the file */
    if (sw != NULL)
        fclose(sw);
}

void saveExt(Token_Node* extTable, char* fileName)
{
    FILE* sw; /* File pointer */
    const size_t length = strlen(fileName) + 5; /* Length of the output file name */
    char* fileStore = NULL; /* Store the modified file name */
    Token_Node* current = extTable; /* Pointer to the current node in the external symbols table */
    
    fileStore = malloc(length);
    if(fileStore == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    } 

    /* Modify the output file name */
    strncpy(fileStore, fileName, length);
    fileStore[length-5] = '.';
    fileStore[length-4] = 'e';
    fileStore[length-3] = 'x';
    fileStore[length-2] = 't';
    fileStore[length-1] = '\0';

    /* Open the output file for writing */
    if ((sw = fopen(fileStore, "w")) == NULL) {
        fprintf(stderr, "Error! Couldn't create or override the file %s", fileStore);
        return;
    }
    free(fileStore);

    /* Write external symbols to the file */
    while (current != NULL) {
        fprintf(sw, "%s     %d\n", current->data->value, current->data->lineNumber);
        current = (Token_Node*)current->next;
    }

    /* Close the file */
    if (sw != NULL)
        fclose(sw);
}

void saveEnt(Simbols_Table* table, char* fileName)
{
    FILE* sw; /* File pointer */
    const size_t length = strlen(fileName) + 5; /* Length of the output file name */
    char* fileStore = NULL; /* Store the modified file name */
    Simbols_Table* current = table; /* Pointer to the current node in the symbol table */

    fileStore = malloc(length);
    if(fileStore == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }

    /* Modify the output file name */
    strncpy(fileStore, fileName, length);
    fileStore[length-5] = '.';
    fileStore[length-4] = 'e';
    fileStore[length-3] = 'n';
    fileStore[length-2] = 't';
    fileStore[length-1] = '\0';

    /* Open the output file for writing */
    if ((sw = fopen(fileStore, "w")) == NULL) {
        fprintf(stderr, "Error! Couldn't create or override the file %s", fileStore);
        return;
    }
    free(fileStore);

    /* Write entry symbols to the file */
    while (current != NULL) {
        if (current->flag == 1)
            fprintf(sw, "%s     %d\n", current->label, current->line);

        current = (Simbols_Table*)current->next;
    }

    /* Close the file */
    if (sw != NULL)
        fclose(sw);
}

