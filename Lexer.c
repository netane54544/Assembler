#include "Lexer.h"
#include "LinkedLists/Linked_List_AsmLines.h"
#include "Utils/Strdup.h"
#include "Utils/CustomStrtok.h"

#define ISREGISTER lastType >= 16 && lastType <= 23 && tempType >= 16 && tempType <= 23
#define LESSTHANTWO 2

/* Find the index of a substring in a string */
int find_substring(char* str, char* substr)
{
    int len_str = strlen(str);
    int len_substr = strlen(substr);
    int i;

    for (i = 0; i <= len_str - len_substr; i++)
    {
        if (strncmp(&str[i], substr, len_substr) == 0)
        {
            return i;
        }
    }

    return -1;
}

/* Check if if our string contains "..." */
bool isString(const char* str)
{
    int len = strlen(str);

    /* Check if the length is at least 2 character*/
    if (len <= LESSTHANTWO)
        return false;

    /* Check first character for " */
    if (str[0] != '"')
        return false;

    /* Check last character for "*/
    if (str[len - 1] != '"')
        return false;

    /* If all conditions are met, it is a quoted string*/
    return true;
}

/* Removes the first and last characters from the given string and returns the modified string. */
char* removeFirstAndLast(char* str)
{
    int len = 0;
    char* result = NULL;

    /* Check if the input is a valid string */
    if (!isString(str))
        return '\0';

    /* Calculate the length of the input string */
    len = strlen(str);

    /* Allocate memory for the modified string */
    result = malloc((len - 1) * sizeof(char));
    if(result ==NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }
    /* Check for memory allocation failure */
    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }

    /* Copy the substring without the first and last characters */
    strncpy(result, str + 1, len - 2);

    /* Null-terminate the modified string */
    result[len - 2] = '\0';

    return result;
}

/* Determines the type of command based on the given token and symbols table. */
enum commands findType(char* token, Simbols_Table* table)
{
    long checkNumber = 0;
    int checkLable, i;
    bool isLable = false;
    bool inTable = false;

    /* Check for command keywords */
    if(strcmp(token, "mov") == 0)
        return mov;
    if(strcmp(token, "cmp") == 0)
        return cmp;
    if(strcmp(token, "add") == 0)
        return add;
    if(strcmp(token, "sub") == 0)
        return sub;
    if(strcmp(token, "lea") == 0)
        return lea;
    if(strcmp(token, "not") == 0)
        return not;
    if(strcmp(token, "clr") == 0)
        return clr;
    if(strcmp(token, "inc") == 0)
        return inc;
    if(strcmp(token, "dec") == 0)
        return dec;
    if(strcmp(token, "jmp") == 0)
        return jmp;
    if(strcmp(token, "bne") == 0)
        return bne;
    if(strcmp(token, "red") == 0)
        return red;
    if(strcmp(token, "prn") == 0)
        return prn;
    if(strcmp(token, "jsr") == 0)
        return jsr;
    if(strcmp(token, "rts") == 0)
        return rts;
    if(strcmp(token, "stop") == 0)
        return stop;
    if(strcmp(token, "@r0") == 0)
        return r0;
    if(strcmp(token, "@r1") == 0)
        return r1;
    if(strcmp(token, "@r2") == 0)
        return r2;
    if(strcmp(token, "@r3") == 0)
        return r3;
    if(strcmp(token, "@r4") == 0)
        return r4;
    if(strcmp(token, "@r5") == 0)
        return r5;
    if(strcmp(token, "@r6") == 0)
        return r6;
    if(strcmp(token, "@r7") == 0)
        return r7;
    if(strcmp(token, ".data") == 0)
        return data;
    if(strcmp(token, ".string") == 0)
        return string;
    if(strcmp(token, ".extern") == 0)
        return externL;
    if(strcmp(token, ".entry") == 0)
        return entryL;

    /* Check if the token is a valid number */
    checkNumber = atol(token);
    if((checkNumber == 0 && *token == '0') || checkNumber != 0)
    {
        if(strchr(token, '.') != NULL)
            return non;

        return number;
    }

    /* Check if the token is a valid string */
    if(isString(token))
        return stringData;

    /* Check for label references in the token */
    checkLable = find_substring(token, ":");
    inTable = isLabel(table, token);
    if(inTable && checkLable == -1)
        return labelCall;

    /*Last check, find by the syntax rules if it is a label*/
    isLable = true;
    if(checkLable != -1 && token[checkLable+1] == '\0')
    {
        for(i = 0; i < checkLable; i++)
        {
            if(!isdigit(token[i]) && !isalpha(token[i]))
            {
                isLable = false;
                break;
            }
        }

        if(isLable)
            return label;
    }
    else
    {
        isLable = false;
    }

    return non; /* Return "non" if the token doesn't match any recognized type */
}

/* Separates a given input string into lines using newline characters as delimiters. */
ASMLines* separateStringByNewlines(const char* inputString)
{
    ASMLines* head = NULL; /* Pointer to the first node in the list */
    char* copyInput = NULL; /* Copy of the input string */
    const char* delimiter = "\n"; /* Delimiter used to split the string */
    char* token = NULL; /* Individual token (line) extracted from the input */

    if (inputString == NULL)
        return NULL;

    /* Create a copy of the input string */
    copyInput = strdup(inputString);

    /* Tokenize the input using newline characters as delimiters */
    token = strtok(copyInput, delimiter);

    /* Iterate through each token (line) */
    while (token != NULL) {
        /* Add the current token (line) to the linked list */
        addNode_asl(&head, token);

        /* Get the next token (line) */
        token = strtok(NULL, delimiter);
    }

    return head; /* Return the head of the linked list */
}

/* Populates the symbols table by analyzing ASMLines and their tokens. */
void getTable(ASMLines* lines, Simbols_Table* table)
{
    const char* delimiters = " ,"; /* Delimiters for tokenization */
    char* cToken; /* Current token being processed */
    ASMLines* tempLine = lines; /* Pointer to the current ASMLines node */
    Token* tempToken = NULL; /* Temporary Token pointer */
    enum commands tempType, lastType; /* Temporary command type and last type */
    int i = 0; /* Line number counter */
    bool isEntry; /* Indicates if the token is an entry */
    bool tokenWasAdded = false; /* Flag to track if token was added to the table */

    /* Iterate through each ASMLines node */
    while (tempLine != NULL)
    {
        /* Tokenize the line using delimiters */
        cToken = custom_strtok(strdup(tempLine->line), delimiters);

        /* Process each token in the current line */
        while (cToken != NULL)
        {
            /* Determine the type of the current token */
            tempType = findType(cToken, table);

            /* Adjust line number count based on the previous and current token types */
            if (ISREGISTER)
                i--;

            /* Handle string data tokens */
            if (tempType == stringData) {
                i += strlen(cToken) - 2;
                tokenWasAdded = false;
            }
                /* Handle label tokens */
            else if (tempType == label) {
                /* Create a temporary Token */
                tempToken = (Token*)malloc(sizeof(Token));
                if(tempToken == NULL)
                {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(0);
                }
                /* Assign values to the temporary Token */
                tempToken->value = cToken;
                tempToken->value[strlen(cToken) - 1] = '\0';
                tempToken->lineNumber = i-- + MEMORYSTART; /* Labels don't increase the count */
                tempToken->tokenType = tempType;

                /* Check if the label already exists in the symbol table */
                if (isLabel(table, tempToken->value))
                {
                    fprintf(stderr, "Error! %s already exists in the symbol table\n", tempToken->value);
                    foundError = true;
                    free(tempToken);
                } else
                {
                    /* Add the temporary Token to the symbol table */
                    addNode_s(table, tempToken->value, tempToken->lineNumber, 0);
                    tokenWasAdded = true;
                }
            }
                /* Handle externL and entryL tokens */
            else if (tempType == externL || tempType == entryL)
            {
                if (tempType == entryL)
                    isEntry = true;
                else
                    isEntry = false;

                /* Move to the next token */
                cToken = custom_strtok(NULL, delimiters);

                if (cToken == NULL) {
                    fprintf(stderr, "Error! At address %d. You are not allowed to call entry or extern without labels\n", i + MEMORYSTART);
                    cToken = custom_strtok(NULL, delimiters);
                    lastType = tempType;
                    continue;
                }

                lastType = tempType;
                tempType = findType(cToken, table);

                /* Delete the last token from the table if tokenWasAdded */
                if (tokenWasAdded)
                    deleteLastNode_s(table);

                /* Process tokens in the context of entryL or externL */
                while (cToken != NULL && (tempType == labelCall || tempType == non)) {
                    if (cToken != NULL) {
                        if (!isEntry && tempType == non) {
                            addNode_s(table, cToken, -1, 2);
                        } else if (tempType == non && isEntry) {
                            break;
                        }

                        cToken = custom_strtok(NULL, delimiters);

                        if (cToken != NULL) {
                            lastType = tempType;
                            tempType = findType(cToken, table);
                        }
                    }
                }

                i--;
                tokenWasAdded = false;
            }
                /* Handle other token types */
            else {
                tokenWasAdded = false;
            }

            /* Update line number count based on token type */
            if (tempType != data && tempType != string)
                i++;

            /* Move to the next token */
            cToken = custom_strtok(NULL, delimiters);
            lastType = tempType;
        }

        /* Move to the next ASMLines node */
        tempLine = (ASMLines*)tempLine->next;
    }
}

/* Searches for unused labels in the symbol table. */
void findUnusedLabel(Simbols_Table* table)
{
    Simbols_Table* current = (Simbols_Table*)table->next; /* Pointer to the next entry in the table */
    Simbols_Table* prev = table; /* Pointer to the previous entry in the table */

    /* If either current or prev is NULL, return */
    if (current == NULL || prev == NULL)
        return;

    /* Iterate through the symbol table */
    while (current != NULL) {
        /* Check if the line numbers match and both flags are not 2 (not used) */
        if (current->line == prev->line && (current->flag != 2 && prev->flag != 2)) {
            fprintf(stderr, "Error! %s is empty. Empty labels are not allowed.\n", prev->label);
            foundError = true;
        }

        /* Update prev and current pointers */
        prev = current;
        current = (Simbols_Table*)current->next;
    }
}

Token_Node* scanner(char* input, Simbols_Table* table)
{
    const char* delimiters = " ,"; /* Delimiters for tokenization */
    char* cToken; /* Current token being processed */
    char* sToken; /* Substring token for stringData */
    char* tempCharArray; /* Temporary character array for single characters in stringData */
    int i = 0, sCounter = 0; /* Line number counter and stringData counter */
    Token_Node* tokens = NULL; /* Linked list to store processed tokens */
    Token* tempToken = NULL; /* Temporary Token pointer */
    ASMLines* line = NULL; /* Pointer to current ASMLines node */
    ASMLines* freeLine = NULL; /* Pointer to free allocated ASMLines nodes */
    enum commands tempType, lastType; /* Temporary command type and last type */
    bool isEntry; /* Indicates if the token is an entry */

    /* Separate the input string into lines and store in line */
    line = separateStringByNewlines(input);
    freeLine = line;

    /* Generate the symbols table from the lines */
    getTable(line, table);

    /* Find unused labels in the symbols table */
    findUnusedLabel(table);

    /* Process each line */
    while (line != NULL) {
        /* Tokenize the line using delimiters */
        cToken = custom_strtok(line->line, delimiters);

        /* Process each token in the current line */
        while (cToken != NULL)
        {
            /* Check for memory limit */
            if (MEMORYLIMIT <= i)
            {
                fprintf(stderr, "Error! Memory limit was exceeded\n");
                foundError = true;
                exit(0);
            }

            /* Determine the type of the current token */
            tempType = findType(cToken, table);

            /* Adjust line number count based on the previous and current token types */
            if (ISREGISTER)
                i--;

            /* Handle non-empty tokens */
            if (tempType != non)
            {
                /* Handle stringData tokens */
                if (tempType == stringData)
                {
                    sToken = removeFirstAndLast(cToken);
                    sCounter = 0;

                    /* Process each character in the stringData */
                    while (sCounter < strlen(sToken)) {
                        tempCharArray = calloc(2, sizeof(char));
                        if(tempCharArray == NULL)
                        {
                            fprintf(stderr, "Memory allocation failed\n");
                            exit(0);
                        }
                        tempCharArray[0] = sToken[sCounter];
                        tempCharArray[1] = '\0';

                        tempToken = (Token*)malloc(sizeof(Token));
                        if (tempToken == NULL)
                        {
                            fprintf(stderr, "Memory allocation failed\n");
                            exit(0);
                        }

                        tempToken->tokenType = tempType;
                        tempToken->value = tempCharArray;
                        tempToken->lineNumber = MEMORYSTART + i++;

                        addNode_t(&tokens, tempToken);

                        sCounter++;
                    }

                    free(sToken);

                    tempToken = (Token*)malloc(sizeof(Token));
                    if(tempToken == NULL)
                    {
                        fprintf(stderr, "Memory allocation failed\n");
                        exit(0);
                    }

                    tempToken->tokenType = tempType;
                    tempToken->value = '\0';
                    tempToken->lineNumber = MEMORYSTART + i;
                    addNode_t(&tokens, tempToken);
                }
                    /* Handle other non-empty tokens */
                else if (tempType != label && tempType != externL && tempType != entryL)
                {
                    tempToken = (Token*)malloc(sizeof(Token));
                    if (tempToken == NULL)
                    {
                        fprintf(stderr, "Memory allocation failed\n");
                        exit(0);
                    }


                    tempToken->tokenType = tempType;
                    tempToken->value = cToken;

                    tempToken->lineNumber = MEMORYSTART + i;

                    addNode_t(&tokens, tempToken);
                }
                    /* Handle externL and entryL tokens */
                else if (tempType == externL || tempType == entryL)
                {
                    if (tempType == entryL)
                        isEntry = true;
                    else
                        isEntry = false;

                    cToken = custom_strtok(NULL, delimiters);

                    /* If NULL, an error was already printed when filling the table */
                    if (cToken == NULL) {
                        cToken = custom_strtok(NULL, delimiters);
                        lastType = tempType;
                        continue;
                    }

                    lastType = tempType;
                    tempType = findType(cToken, table);

                    /* Handle non-existent labels */
                    if (tempType != labelCall) {
                        fprintf(stderr, "Error! At address %d. You are not allowed to use entry on a non-existent label\n", i + MEMORYSTART);
                        foundError = true;
                    }

                    /* Process labels for entryL or externL */
                    while (cToken != NULL && tempType == labelCall)
                    {
                        if (isEntry)
                            findInTable_s(table, cToken)->flag = 1;
                        else
                            findInTable_s(table, cToken)->flag = 2;

                        cToken = custom_strtok(NULL, delimiters);

                        if (cToken != NULL)
                        {
                            lastType = tempType;
                            tempType = findType(cToken, table);
                        }
                    }

                    i--;
                }
                else
                {
                    i--;
                }
            }
                /* Handle non-recognized tokens */
            else if (tempType == non)
            {
                fprintf(stderr, "At address %d. %s is not recognized\n", i + MEMORYSTART, cToken);
                foundError = true;
            }

            /* Update line number count based on token type */
            if (tempType != data && tempType != string)
                i++;
            cToken = custom_strtok(NULL, delimiters);
            lastType = tempType;
        }

        /* Move to the next ASMLines node */
        line = (ASMLines*)line->next;
    }

    /* Check for memory limit before continuing */
    if (MEMORYLIMIT <= i)
    {
        fprintf(stderr, "Error! Memory limit was exceeded\n");
        foundError = true;
        exit(0);
    }

    /* Delete the linked list of ASMLines */
    deleteList_asm(freeLine);

    return tokens; /* Return the processed tokens */
}
