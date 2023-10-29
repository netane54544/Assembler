#include "toolFunctions.h"
#include "../Utils/Strdup.h"
#include "../Utils/CStrcat.h"
#include "../Utils/Globals.h"

#define ENDMCROSIZE 7
#define STARTMCROSIZE 4
#define STARTMCROSIZEANDSPACE 5
#define NOTFOUND -1

/*This function checks if the string is a valid "endmcro" statement.*/
bool checkEndMcro(const char* str)
{
    /*Skip initial whitespace*/
    while (isspace(*str))
        str++;

    /* Check if the string starts with "mcro"*/
    if (strncmp(str, "endmcro", ENDMCROSIZE) != 0)
        return false;

    /* Move past "mcro" and any additional whitespace*/
    str += ENDMCROSIZE;
    while (isspace(*str))
        str++;

    /* Check that there is exactly one word and no additional characters*/
    if (*str != '\0')
        return false;

    return true;
}

/* this function checks if the string starts with the text "mcro" and has exactly one word after it.
A macro definition starts with the text "mcro" followed by a word.*/
bool startsWithMcroAndOneWord(const char* str)
{
    bool hasSpace = false;

    /* Skip initial whitespace */
    while (isspace(*str))
        str++;

    /* Check if the string starts with exactly "mcro" followed by a space or the end of the string */
    if ((strncmp(str, "mcro", STARTMCROSIZE) != 0) || (str[STARTMCROSIZE] != ' ' && str[STARTMCROSIZE] != '\0'))
        return false;

    /* Move past "mcro" and the following space */
    str += STARTMCROSIZEANDSPACE;

    /* Check for exactly one word after "mcro" */
    while (*str != '\0' && !isspace(*str))
    {
        hasSpace = true;
        str++;
    }

    /* Skip trailing whitespace after the word */
    while (isspace(*str))
        str++;

    /* Check that there is exactly one word and no additional characters */
    if (!hasSpace || *str != '\0')
        return false;

    return true;
}

bool isWhitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/*this function replaces the word at the given index in the string with the new word.*/
void replaceWord(char** originalString, int index, char *newWord)
{
    char* myString = *originalString;
    char* outputString = NULL;
    int originalLength = strlen(myString);
    int newWordLength = strlen(newWord);
    int partBeforeLength;
    int partAfterLength;
    int start = index;
    int end = index;
    int outputLength;

    /* Find the start and end of the word to be replaced*/
    while (start > 0 && !isWhitespace(myString[start - 1]))
        start--;


    while (end < originalLength && !isWhitespace(myString[end]))
        end++;

    /* Calculate the lengths of the parts before and after the replaced word*/
    partBeforeLength = start;
    partAfterLength = originalLength - end;

    /* Calculate the total length of the output string*/
    outputLength = partBeforeLength + newWordLength + partAfterLength;

    outputString = malloc(outputLength + 1);
    if (outputString == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }

    /*Copy the part before the replaced word*/
    strncpy(outputString, myString, partBeforeLength);
    outputString[partBeforeLength] = '\0';

    /* Concatenate the new word*/
    strcat(outputString, newWord);

    /* Concatenate the rest of the original string*/
    strcat(outputString, myString + end);

    *originalString = outputString;
}
/*Removes the substring from the given string.*/
void remove_substring(char* str, int start_index, int end_index) {
    /* Get the length of the input string. */
    int len_str = strlen(str);

    /* Check for invalid input: start_index < 0, end_index >= len_str, or start_index > end_index. */
    if (start_index < 0 || end_index >= len_str || start_index > end_index)
    {
        fprintf(stderr, "Invalid input.\n");
        return;
    }

    /* Shift all characters after the matched substring to the left by the length of the matched substring. */
    memmove(&str[start_index], &str[end_index + 1], (len_str - end_index) * sizeof(char));
}

/*Removes comments from the given string.*/
void removeComments(char* str) {
    int inComment = 0;/*flag set to false*/
    char* src = str;/* so we don't lose the address*/
    char* dest = str;

    while (*src != '\0')
    {/*not null*/
        if (*src == ';' && !inComment)
        {/*in the line we find ; + incomment flag is set to false*/
            inComment = 1; /* Start of comment*/
        } else if (*src == '\n' && inComment) {/*else we will look for /n or untill we get to one*/
            inComment = 0; /*End of comment*/
            src++; /*we move src like in array manner to the next slot*/
            continue;/*we don't want to continue the rest of the code*/
        }
        /* copies the character at the src pointer to the memory location pointed to by dest=destion.
         * It then increments both the src and dest pointers.*/
        if (!inComment)/*in case inComment is true means we need to continue untill */
            *dest++ = *src;/*firstly we copy the address of src to dest and after wards we incement dest which in return incements src*/
        src++;/*moves the src pointer to the next character in the input string.
 * This process ensures that the character is copied and both pointers are correctly advanced for the next iteration.*/
    }

    *dest = '\0'; /* Terminate the resulting string*/
}

/*Removes empty lines from the given string.*/
void removeEmptyLines(char* str) {
    char* src = str; /* Pointer to traverse the input string. */
    char* dest = str; /* Pointer to store the modified string with empty lines removed. */
    int inLine = 0; /* Flag to indicate whether the current line has any non-whitespace characters. */

    while (*src != '\0')
    {
        /* Check if the current character is a newline or tab character. */
        if (*src == '\n' || *src == '\t')
        {
            if (!inLine)
            {
                src++;
                continue; /* Skip the empty line or line with only tabs */
            }
            inLine = 0; /* Reset the inLine flag for the next line */
        } else if (*src != ' ' && *src != '\t') {
            inLine = 1; /* Set the inLine flag if a non-whitespace character is encountered */
        }

        *dest++ = *src++; /* Copy the current character to the destination string. */
    }

    *dest = '\0'; /* Terminate the resulting string */
}

/* Find a substring index inside a string */
int find_sub(const char *str, const char *substring)
{
    int strLen = strlen(str);
    int subLen = strlen(substring);
    int i, j;

    for (i = 0; i <= strLen - subLen; i++)
    {
        for (j = 0; j < subLen; j++)
        {
            if (str[i + j] != substring[j])
                break;
        }

        if (j == subLen)
            return i;  /* Substring found starting at index i */
    }

    return -1; /* Substring not found */
}

/*Finds the position of the first occurrence of the substring `needle` in the string `haystack`*/
/* This only return the index if the substring is of type mcro and not ssmcro, this is why we need find_sub */
int find_string_position(const char* haystack, const char* needle) {
    int needle_length;
    int haystack_length;
    int i;

    if (haystack == NULL || needle == NULL)
        return -1;

    needle_length = strlen(needle);
    haystack_length = strlen(haystack);

    for (i = 0; i <= haystack_length - needle_length; i++) {
        char prev_char = i > 0 ? haystack[i - 1] : '\0';
        char next_char = haystack[i + needle_length];

        if ((i == 0 || isspace(prev_char) || ispunct(prev_char)) &&
            (i + needle_length == haystack_length || isspace(next_char) || ispunct(next_char)))
        {
            if (strncmp(haystack + i, needle, needle_length) == 0)
                return i;
        }
    }

    return -1;
}


void replaceTargetWithReplacement(char** text, const char* target, const char* replacement) {
    int targetPos;
    char* tempText = NULL;

    while ((targetPos = find_string_position(*text, target)) != NOTFOUND)
    {
        tempText = *text;
        replaceWord(text, targetPos, (char *)replacement);
        free(tempText);
    }
}


/*Removes all whitespace characters from the given string.*/
char *remove_whitespace(char *string)
{
    char *ptr = NULL;
    char *src = NULL;
    char *new_string = malloc(strlen(string) + 1); /* Allocate memory for the new string. */

    if (new_string == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(0);
    }

    ptr = new_string; /* Pointer to write the new string to. */
    src = string; /* Pointer to traverse the original string. */

    /* Loop through the original string until the end. */
    while (*src != '\0') {

        /* Check if the current character is whitespace. */
        if (!isspace(*src)) {
            /* Copy the current character to the new string. */
            *ptr++ = *src;
        }

        src++; /* Move to the next character in the original string. */
    }

    /* Terminate the new string. */
    *ptr = '\0';

    /* Return the new string. */
    return new_string;
}

/*Finds the word "mcro" in a string and returns the index of the first character of the line that it was found on.*/
int find_mcro(const char *string) 
{
    int index = 0;
    int line_start = 0;

    /* Loop through the characters in the string */
    while (string[index] != '\0') {
        /* Check if the substring "mcro" is found from the current position */
        if (strncmp(&string[index], "mcro", STARTMCROSIZE) == 0)
        {
            /* Find the index of the start of the line*/
            line_start = index;
            while (line_start > 0 && string[line_start] != '\n')
                line_start--;

            return line_start;
        }

        /* Move to the next character */
        index++;
    }

    /* The word "mcro" was not found in the string */
    return NOTFOUND;
}

/*Finds all macro definitions in the given string and saves them to a linked list.*/
Linked_mcro* find_and_save_macros(char *text)
{
    int startMacro = 0;
    int last_endmcro = 0;
    Linked_mcro *head = NULL;
    Linked_mcro *current = NULL;
    char *token = strdup(text);
    /* Get the first token.*/
    token = strtok(token, "\n");
    

    while (token != NULL) {
        /* Check if the token is the start of a macro definition.*/

        if(!startsWithMcroAndOneWord(token) && find_sub(token, "mcro") != NOTFOUND)
        {
            printf("Error: %s\n", token);
            foundError = true;
            break;
        }
        else if (find_string_position(token, "mcro") != NOTFOUND)
        {
            /* Create a new node for the macro definition.*/
	    if (head == NULL) 
	    {
                head = (Linked_mcro*)createNode("",remove_whitespace(token + STARTMCROSIZEANDSPACE));
                current = head;
            } else 
	    {
                current->next = (Linked_mcro*)createNode("",remove_whitespace(token + STARTMCROSIZEANDSPACE));
            }

            if(current == NULL)
            {
		return NULL;
	    }
            startMacro = find_mcro(text);

            last_endmcro += strlen(token);
            /* Get the next token. */
            token = strtok(NULL, "\n");

            /* While the next token is not the end of the macro definition, keep appending it to the node. */
            while (token != NULL && find_string_position(token, "endmcro") == NOTFOUND)
            {
                current->text = alloc_strcat(current->text, token);
                current->text = alloc_strcat(current->text, "\n");
                last_endmcro += strlen(token) + 1;
                token = strtok(NULL, "\n");
            }

            if(find_string_position(token, "endmcro") != NOTFOUND && !checkEndMcro(token))
            {
                printf("Error: %s\n", token);
                foundError = true;
                break;
            }

            last_endmcro += strlen(token) + 1;
            remove_substring(text, startMacro, last_endmcro + startMacro);
            last_endmcro = 0;
        }

        token = strtok(NULL, "\n");
    }

    return head;
}

char* insertSpaceBetweenDoubleCommas(const char* str)
{
    size_t length = 0;
    char* result = NULL;
    char* readPtr = NULL;
    char* writePtr = NULL;

    if (str == NULL)
        return NULL; /* Handle invalid input*/

    length = strlen(str);
    result = (char*)malloc((2 * length + 1) * sizeof(char)); /* Space for extra characters and null terminator*/

    if(result == NULL)
    {
        fprintf(stderr, "Error allocating memory");
        exit(0);
    }

    readPtr = (char*)str;
    writePtr = result;

    while (*readPtr) { /*pointer that points to a valid memory address evaluates to true, and a pointer that is NULL evaluates to false.*/
        if (*readPtr == ',' && *(readPtr + 1) == ',') {
            *writePtr++ = *readPtr++;/*character is accessed, and then the pointers are incremented to point to the next character in the memory.*/
            *writePtr++ = ' ';/*we reach this line. Here, we assign a space character (' ') to the location pointed to by writePtr and then increment writePtr to point to the next character in the destination string.*/
        } else
        {
            *writePtr++ = *readPtr++;
        }
    }

    *writePtr = '\0';
    return result;
}
