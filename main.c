#include "Lexer.h"
#include "Parser.h"
#include "Utils/Globals.h"
#include "PreProcessor/pre-processor.h"

#define RESETVAR sT->flag = 0; sT->label = NULL; sT->next = NULL; sT->line = -10; foundError = false; dataCounter = 0; imageCounter = 0; /* Resets our variables for the next file */
#define MINARGS 2

int main(int argc, char* argv[]) {
    /* Initialize counters for data and image processing */
    size_t dataCounter = 0;
    size_t imageCounter = 0;
    /* Counter for iteration through command-line arguments */
    int i;
    /* Pointer to store input file content */
    char* input = NULL;
    /* Pointer to the symbols table structure */
    Simbols_Table* sT = NULL;
    /* Pointer to the head of the token linked list */
    Token_Node* head = NULL;

    /* Check if the program was called with the correct number of arguments */
    if (argc < MINARGS) {
        fprintf(stderr, "Error! Not enough arguments\n");
        exit(0);
    }

    /* Iterate through the command-line arguments (file names) */
    for (i = 1; i < argc; i++) {
        /* Allocate memory for the symbols table */
        sT = (Simbols_Table*) malloc(sizeof(Simbols_Table));

        /* Check for memory allocation error */
        if (sT == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            exit(0);
        }

        /* Reset variables within the symbols table */
        RESETVAR

        /* Print the current file being processed */
        printf("File \"%s\":\n", argv[i]);

        /* Pre-process the input file to handle any initial processing */
        input = pre_process(argv[i]);

        /* If there is an error in pre-processing, move to the next file */
        if (input == NULL) {
            /* Clean up symbols table memory and continue to the next iteration */
            deleteList_s(sT);
            continue;
        }

        /* Scan the input content and generate a linked list of tokens */
        head = scanner(input, sT);

#if DEBUG
        /* Debug mode: Print the token list and symbols table */
        printList_t(head);
        printf("\n");
        printList_s(sT);
        printf("\n");
#endif

        /* If no errors were found during scanning */
        if (!foundError) {
            /* Parse the commands and update data and image counters */
            parseCommand(head, sT, &dataCounter, &imageCounter);
#if DEBUG
	    printf("\nImage Counter: %d, Data Counter: %d\n", imageCounter, dataCounter);
#endif
        }

        /* Clean up memory allocated for token list, symbols table, and input */
        deleteList_t(head);
        deleteList_s(sT);
        free(input);
    }

    return 0;
}
