#include "pre-processor.h"
#include "toolFunctions.h"
#include "../Utils/Globals.h"
#include "../Utils/Strdup.h"
#include "../Utils/CStrcat.h"

/*This function checks if a name is in a linked list.*/
bool isNameInList(const char* nameToCheck, Linked_mcro* listHead)
{
    int nameCounter = 0;  /* Initialize a counter to keep track of the number of times we find the name. */
    Linked_mcro* current = listHead;  /* Initialize a pointer to the current node in the linked list. */

    /* Iterate through the linked list. */
    while (current != NULL)
    {
        /* If we find the name, increment the counter. */
        if (strcmp(nameToCheck, (char*)current->name) == 0)
            nameCounter++;

        /* Move to the next node in the linked list. */
        current = (Linked_mcro*)current->next;
    }

    /* If the counter is greater than 1, then the name was found more than once in the linked list. */
    if (nameCounter > 1)
        return true;

    /* Otherwise, the name was not found in the linked list. */
    return false;
}

/*This function checks if the length of a line of text is greater than a certain limit.*/
bool check_line_length(char *text)
{
    char *line = text;  /* Initialize a pointer to the start of the line. */
    int line_length = 0;  /* Initialize a counter to keep track of the line length. */

    /* Iterate through the text, one character at a time. */
    while (*line != '\0') {
        /* If we encounter a newline character, reset the line length counter. */
        if (*line == '\n')
        {
            if (line_length >= LIMIT_FOR_CHARACTERS_IN_LINES)
                return false;

            line_length = 0;
        } else
        {
            /* Otherwise, increment the line length counter. */
            line_length++;
        }

        line++;
    }

    /* If the line length is still greater than the limit, return false. */
    if (line_length >= LIMIT_FOR_CHARACTERS_IN_LINES)
        return false;

    /* Otherwise, the line length is within the limit, so return true. */
    return true;
}

/*This function reads the contents of a file into a buffer.*/
char* readFile(char* filename)
{
    FILE *fp;
    long length = 0; /*Variable to store the length of the C code file*/
    char *buffer = NULL;

    /*Attempt to open the input C code file for reading*/
    if ((fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error! Couldn't open %s\n", filename);
        foundError = true;
        return NULL;
    }

    /*Get the length of the input C code file*/
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    /*Allocate memory for the buffer to store the content of the input C code file*/
    buffer = calloc(length + 1, sizeof(char));
    /* Check if memory allocation was successful*/
    if (buffer == NULL)
    {
        fprintf(stderr, "Error allocating memory");
        exit(0);
    }

    /* Read the content of the input C code file into the buffer*/
    if (buffer)
        fread(buffer, 1, length, fp);
    /*Close the input C code file*/
    if (fp != NULL)
        fclose(fp);

    /*Null-terminate the buffer*/
    buffer[length] = '\0';

    return buffer;
}

/*This function is the main function of the preprocessor.*/
char* pre_process(char* filename)
{
     /* File pointer for reading input C code files*/
    FILE *fw; /* File pointer for writing preprocessed C code*/
    char *buffer = NULL; /*Buffer to store the content of the C code file*/
    char* tempBuffer = NULL;
    char* fullFileName = NULL;
    Linked_mcro *getList = NULL; /*Linked list to store extracted macro names and words*/
    Linked_mcro *current = NULL; /*Pointer to traverse the linked list*/

    fullFileName = strdup(filename);
    fullFileName = alloc_strcat(fullFileName, ".as");
    buffer = readFile(fullFileName);

    /* check that buffer isn't null */
    if (buffer)
    {
        check_line_length(buffer);

        /*Extract macro names and words from the C code and store them in the linked list*/
        getList = find_and_save_macros(buffer);

        if(getList != NULL)
        {
            current = getList;
            while(current != NULL)
            {
                if(isNameInList(current->name, current))
                {
                    fprintf(stderr, "The name \"%s\" is already in the list.\n", current->name);
                    freeList(getList);
                    free(fullFileName);
                    return NULL;
                }

                current = (Linked_mcro*)current->next;
            }

            current = getList;
        }
        /*Remove comments from /.mnb the C code*/
        removeComments(buffer);

        /*Replace occurrences of macro names with their corresponding macro words*/
        if(getList != NULL)
        {
            while (current != NULL)
            {
                replaceTargetWithReplacement(&buffer, current->name, current->text);
                current = (Linked_mcro*)current->next;
            }
        }

        /* we finished using getList, so we need to free the memory */
        if(getList != NULL)
            freeList(getList);

        /*Remove empty lines and tabs from the C code*/
        removeEmptyLines(buffer);
        /*Insert a space between consecutive double commas in the C code*/
        tempBuffer = buffer;
        buffer = insertSpaceBetweenDoubleCommas(buffer);
        free(tempBuffer);
        SAVEM/*switchs the extention of the file to save as am*/

        if(foundError)
        {
            free(buffer);
            free(fullFileName);
            return NULL;
        }

        /*Attempt to open the output preprocessed file for writing*/
        if ((fw = fopen(fullFileName, "w")) == NULL)
        {
            fprintf(stderr, "Error! Couldn't create the file for %s\n", filename);
            free(buffer);
            free(fullFileName);
            return NULL;
        }

        /*Write the preprocessed C code to the output preprocessed file*/
        fprintf(fw, "%s", buffer);

        if ((fw = fopen(fullFileName, "r")) == NULL)
        {
            fprintf(stderr, "Error! Couldn't create the file for %s\n", filename);
            free(buffer);
            free(fullFileName);
            return NULL;
        }

        /* Close the output preprocessed file*/
        if (fw != NULL)
            fclose(fw);

        /* fileName is a global */
        fileName = filename;

        return buffer;
    }

    return NULL;
}
